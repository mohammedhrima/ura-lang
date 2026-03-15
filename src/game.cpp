#include "raylib.h"
#include <cstdlib>
#include <ctime>
#include <cstring>

// ── Constants ────────────────────────────────────────────────────────────────
static const int BOARD_COLS   = 10;
static const int BOARD_ROWS   = 20;
static const int CELL          = 30;          // pixels per cell
static const int BOARD_X       = 20;
static const int BOARD_Y       = 20;
static const int PANEL_X       = BOARD_X + BOARD_COLS * CELL + 20;  // 370
static const int WIN_W         = PANEL_X + 160;  // 530
static const int WIN_H         = BOARD_Y + BOARD_ROWS * CELL + 20;  // 640

// ── Piece types ──────────────────────────────────────────────────────────────
enum PieceType { I = 0, O, T, S, Z, J, L, PIECE_COUNT };

// SHAPES[piece][rotation][row] — 4 rows, each row is a 4-bit mask (bit3=col0)
static const unsigned char SHAPES[7][4][4] = {
    // I
    { {0b0000, 0b1111, 0b0000, 0b0000},
      {0b0010, 0b0010, 0b0010, 0b0010},
      {0b0000, 0b1111, 0b0000, 0b0000},
      {0b0010, 0b0010, 0b0010, 0b0010} },
    // O
    { {0b0110, 0b0110, 0b0000, 0b0000},
      {0b0110, 0b0110, 0b0000, 0b0000},
      {0b0110, 0b0110, 0b0000, 0b0000},
      {0b0110, 0b0110, 0b0000, 0b0000} },
    // T
    { {0b0100, 0b1110, 0b0000, 0b0000},
      {0b0100, 0b0110, 0b0100, 0b0000},
      {0b0000, 0b1110, 0b0100, 0b0000},
      {0b0100, 0b1100, 0b0100, 0b0000} },
    // S
    { {0b0110, 0b1100, 0b0000, 0b0000},
      {0b0100, 0b0110, 0b0010, 0b0000},
      {0b0110, 0b1100, 0b0000, 0b0000},
      {0b0100, 0b0110, 0b0010, 0b0000} },
    // Z
    { {0b1100, 0b0110, 0b0000, 0b0000},
      {0b0010, 0b0110, 0b0100, 0b0000},
      {0b1100, 0b0110, 0b0000, 0b0000},
      {0b0010, 0b0110, 0b0100, 0b0000} },
    // J
    { {0b1000, 0b1110, 0b0000, 0b0000},
      {0b0110, 0b0100, 0b0100, 0b0000},
      {0b0000, 0b1110, 0b0010, 0b0000},
      {0b0100, 0b0100, 0b1100, 0b0000} },
    // L
    { {0b0010, 0b1110, 0b0000, 0b0000},
      {0b0100, 0b0100, 0b0110, 0b0000},
      {0b0000, 0b1110, 0b1000, 0b0000},
      {0b1100, 0b0100, 0b0100, 0b0000} },
};

static const Color PIECE_COLORS[7] = {
    SKYBLUE,    // I
    YELLOW,     // O
    PURPLE,     // T
    GREEN,      // S
    RED,        // Z
    BLUE,       // J
    ORANGE,     // L
};

// ── Structs ───────────────────────────────────────────────────────────────────
struct Piece {
    PieceType type;
    int       rotation;
    int       col;  // board column of top-left of 4x4 bounding box
    int       row;  // board row of top-left
};

struct GameState {
    int   board[BOARD_ROWS][BOARD_COLS]; // 0=empty, 1..7=color index
    Piece current;
    Piece next;
    int   score;
    int   level;
    int   linesCleared;
    float tickTimer;
    float tickInterval;
    bool  gameOver;
    bool  paused;
};

// ── Helpers ───────────────────────────────────────────────────────────────────
static inline bool CellOccupied(PieceType t, int rot, int r, int c) {
    return (SHAPES[(int)t][rot][r] >> (3 - c)) & 1;
}

static PieceType RandomPieceType() {
    return (PieceType)(rand() % (int)PIECE_COUNT);
}

static Piece SpawnPiece(PieceType type) {
    Piece p;
    p.type     = type;
    p.rotation = 0;
    p.col      = 3;
    p.row      = 0;
    return p;
}

// ── Collision ─────────────────────────────────────────────────────────────────
static bool IsValidPosition(const GameState& gs, const Piece& p) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (!CellOccupied(p.type, p.rotation, r, c)) continue;
            int br = p.row + r;
            int bc = p.col + c;
            if (br < 0 || br >= BOARD_ROWS) return false;
            if (bc < 0 || bc >= BOARD_COLS) return false;
            if (gs.board[br][bc] != 0)      return false;
        }
    }
    return true;
}

// ── Scoring ───────────────────────────────────────────────────────────────────
static float TickIntervalForLevel(int level) {
    float t = 0.8f - (level - 1) * 0.07f;
    return t < 0.05f ? 0.05f : t;
}

static void UpdateScore(GameState& gs, int lines) {
    static const int LINE_POINTS[5] = { 0, 100, 300, 500, 800 };
    if (lines > 4) lines = 4;
    gs.score        += LINE_POINTS[lines] * gs.level;
    gs.linesCleared += lines;
    gs.level         = gs.linesCleared / 10 + 1;
    gs.tickInterval  = TickIntervalForLevel(gs.level);
}

// ── Line clearing ─────────────────────────────────────────────────────────────
static int ClearLines(GameState& gs) {
    int cleared = 0;
    for (int r = BOARD_ROWS - 1; r >= 0; ) {
        bool full = true;
        for (int c = 0; c < BOARD_COLS; c++) {
            if (gs.board[r][c] == 0) { full = false; break; }
        }
        if (full) {
            // shift everything above down
            for (int rr = r; rr > 0; rr--)
                memcpy(gs.board[rr], gs.board[rr - 1], sizeof(int) * BOARD_COLS);
            memset(gs.board[0], 0, sizeof(int) * BOARD_COLS);
            cleared++;
            // don't increment r — re-check same row after shift
        } else {
            r--;
        }
    }
    return cleared;
}

// ── Locking ───────────────────────────────────────────────────────────────────
static void LockPiece(GameState& gs) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (!CellOccupied(gs.current.type, gs.current.rotation, r, c)) continue;
            int br = gs.current.row + r;
            int bc = gs.current.col + c;
            if (br < 0) { gs.gameOver = true; return; }
            gs.board[br][bc] = (int)gs.current.type + 1;
        }
    }
    int lines = ClearLines(gs);
    if (lines > 0) UpdateScore(gs, lines);

    gs.current = gs.next;
    gs.next    = SpawnPiece(RandomPieceType());

    if (!IsValidPosition(gs, gs.current))
        gs.gameOver = true;
}

// ── Movement ──────────────────────────────────────────────────────────────────
static bool MoveLeft(GameState& gs) {
    Piece p = gs.current; p.col--;
    if (!IsValidPosition(gs, p)) return false;
    gs.current = p; return true;
}

static bool MoveRight(GameState& gs) {
    Piece p = gs.current; p.col++;
    if (!IsValidPosition(gs, p)) return false;
    gs.current = p; return true;
}

static bool MoveDown(GameState& gs) {
    Piece p = gs.current; p.row++;
    if (!IsValidPosition(gs, p)) return false;
    gs.current = p; return true;
}

static void RotateCW(GameState& gs) {
    Piece p = gs.current;
    p.rotation = (p.rotation + 1) % 4;
    // Try position as-is, then wall kicks
    static const int kicks[4][2] = { {0,0}, {0,-1}, {0,1}, {0,-2} };
    for (auto& k : kicks) {
        Piece q = p; q.col += k[1]; q.row += k[0];
        if (IsValidPosition(gs, q)) { gs.current = q; return; }
    }
}

static void HardDrop(GameState& gs) {
    int dropped = 0;
    while (MoveDown(gs)) dropped++;
    gs.score += dropped * 2;
    LockPiece(gs);
}

static int GhostRow(const GameState& gs) {
    Piece p = gs.current;
    while (true) {
        Piece next = p; next.row++;
        if (!IsValidPosition(gs, next)) break;
        p = next;
    }
    return p.row;
}

// ── Input ─────────────────────────────────────────────────────────────────────
static void HandleInput(GameState& gs) {
    if (gs.gameOver) {
        if (IsKeyPressed(KEY_R)) {
            // reinit handled in main loop
        }
        return;
    }
    if (IsKeyPressed(KEY_P)) { gs.paused = !gs.paused; return; }
    if (gs.paused) return;

    if (IsKeyPressed(KEY_LEFT))  MoveLeft(gs);
    if (IsKeyPressed(KEY_RIGHT)) MoveRight(gs);
    if (IsKeyPressed(KEY_UP))    RotateCW(gs);
    if (IsKeyPressed(KEY_SPACE)) RotateCW(gs);
    if (IsKeyPressed(KEY_DOWN))  HardDrop(gs);
}

// ── Init ──────────────────────────────────────────────────────────────────────
static void InitGame(GameState& gs) {
    memset(gs.board, 0, sizeof(gs.board));
    gs.score        = 0;
    gs.level        = 1;
    gs.linesCleared = 0;
    gs.tickTimer    = 0.0f;
    gs.tickInterval = TickIntervalForLevel(1);
    gs.gameOver     = false;
    gs.paused       = false;
    gs.current      = SpawnPiece(RandomPieceType());
    gs.next         = SpawnPiece(RandomPieceType());
}

// ── Update ────────────────────────────────────────────────────────────────────
static void UpdateGame(GameState& gs, float dt) {
    gs.tickTimer += dt;
    if (gs.tickTimer >= gs.tickInterval) {
        gs.tickTimer = 0.0f;
        if (!MoveDown(gs)) LockPiece(gs);
    }
}

// ── Draw helpers ──────────────────────────────────────────────────────────────
static void DrawCell(int boardCol, int boardRow, Color color) {
    int x = BOARD_X + boardCol * CELL;
    int y = BOARD_Y + boardRow * CELL;
    DrawRectangle(x + 1, y + 1, CELL - 2, CELL - 2, color);
    // subtle inner highlight
    DrawRectangleLines(x + 1, y + 1, CELL - 2, CELL - 2,
                       ColorBrightness(color, 0.3f));
}

static void DrawPieceAt(PieceType type, int rot, int boardCol, int boardRow, Color color) {
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            if (CellOccupied(type, rot, r, c))
                DrawCell(boardCol + c, boardRow + r, color);
}

// ── Rendering ─────────────────────────────────────────────────────────────────
static void DrawBoard(const GameState& gs) {
    // Background
    DrawRectangle(BOARD_X, BOARD_Y,
                  BOARD_COLS * CELL, BOARD_ROWS * CELL,
                  Color{20, 20, 30, 255});

    // Grid lines
    for (int r = 0; r <= BOARD_ROWS; r++)
        DrawLine(BOARD_X, BOARD_Y + r * CELL,
                 BOARD_X + BOARD_COLS * CELL, BOARD_Y + r * CELL,
                 Color{40, 40, 55, 255});
    for (int c = 0; c <= BOARD_COLS; c++)
        DrawLine(BOARD_X + c * CELL, BOARD_Y,
                 BOARD_X + c * CELL, BOARD_Y + BOARD_ROWS * CELL,
                 Color{40, 40, 55, 255});

    // Locked cells
    for (int r = 0; r < BOARD_ROWS; r++)
        for (int c = 0; c < BOARD_COLS; c++)
            if (gs.board[r][c] != 0)
                DrawCell(c, r, PIECE_COLORS[gs.board[r][c] - 1]);

    // Board border
    DrawRectangleLines(BOARD_X, BOARD_Y,
                       BOARD_COLS * CELL, BOARD_ROWS * CELL,
                       Color{180, 180, 200, 255});
}

static void DrawGhost(const GameState& gs) {
    int ghostRow = GhostRow(gs);
    if (ghostRow == gs.current.row) return; // already at floor
    Color gc = PIECE_COLORS[(int)gs.current.type];
    gc.a = 60;
    DrawPieceAt(gs.current.type, gs.current.rotation,
                gs.current.col, ghostRow, gc);
}

static void DrawCurrentPiece(const GameState& gs) {
    DrawPieceAt(gs.current.type, gs.current.rotation,
                gs.current.col, gs.current.row,
                PIECE_COLORS[(int)gs.current.type]);
}

static void DrawHUD(const GameState& gs) {
    int px = PANEL_X;
    int py = BOARD_Y;

    // Panel background
    DrawRectangle(px - 5, py, 155, BOARD_ROWS * CELL,
                  Color{25, 25, 38, 255});

    // Score
    DrawText("SCORE",      px, py + 10,  16, LIGHTGRAY);
    DrawText(TextFormat("%d", gs.score), px, py + 30, 22, WHITE);

    // Level
    DrawText("LEVEL",      px, py + 70,  16, LIGHTGRAY);
    DrawText(TextFormat("%d", gs.level), px, py + 90, 22, WHITE);

    // Lines
    DrawText("LINES",      px, py + 130, 16, LIGHTGRAY);
    DrawText(TextFormat("%d", gs.linesCleared), px, py + 150, 22, WHITE);

    // Next piece label
    DrawText("NEXT",       px, py + 200, 16, LIGHTGRAY);

    // Next piece preview box (4x4 cells at half size = 20px each)
    int bx = px;
    int by = py + 220;
    int cs = 20;
    DrawRectangle(bx, by, 4 * cs, 4 * cs, Color{20, 20, 30, 255});
    DrawRectangleLines(bx, by, 4 * cs, 4 * cs, Color{180, 180, 200, 255});

    // draw next piece in preview
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (!CellOccupied(gs.next.type, gs.next.rotation, r, c)) continue;
            int x = bx + c * cs + 1;
            int y = by + r * cs + 1;
            Color col = PIECE_COLORS[(int)gs.next.type];
            DrawRectangle(x, y, cs - 2, cs - 2, col);
            DrawRectangleLines(x, y, cs - 2, cs - 2,
                               ColorBrightness(col, 0.3f));
        }
    }

    // Controls hint
    DrawText("← → move",    px, py + 370, 12, DARKGRAY);
    DrawText("↑/SPC rotate", px, py + 386, 12, DARKGRAY);
    DrawText("↓  hard drop", px, py + 402, 12, DARKGRAY);
    DrawText("P   pause",    px, py + 418, 12, DARKGRAY);
}

static void DrawGameOver(const GameState& gs) {
    // Overlay
    DrawRectangle(BOARD_X, BOARD_Y,
                  BOARD_COLS * CELL, BOARD_ROWS * CELL,
                  Color{0, 0, 0, 170});

    int cx = BOARD_X + BOARD_COLS * CELL / 2;
    int cy = BOARD_Y + BOARD_ROWS * CELL / 2;

    DrawText("GAME OVER",
             cx - MeasureText("GAME OVER", 28) / 2,
             cy - 50, 28, RED);

    const char* scoreStr = TextFormat("Score: %d", gs.score);
    DrawText(scoreStr,
             cx - MeasureText(scoreStr, 18) / 2,
             cy - 10, 18, WHITE);

    DrawText("Press R to restart",
             cx - MeasureText("Press R to restart", 14) / 2,
             cy + 20, 14, LIGHTGRAY);
}

static void DrawPauseOverlay() {
    DrawRectangle(BOARD_X, BOARD_Y,
                  BOARD_COLS * CELL, BOARD_ROWS * CELL,
                  Color{0, 0, 0, 140});

    int cx = BOARD_X + BOARD_COLS * CELL / 2;
    int cy = BOARD_Y + BOARD_ROWS * CELL / 2;
    DrawText("PAUSED",
             cx - MeasureText("PAUSED", 28) / 2,
             cy - 14, 28, YELLOW);
}

static void RenderFrame(const GameState& gs) {
    ClearBackground(Color{15, 15, 22, 255});
    DrawBoard(gs);
    if (!gs.gameOver) {
        DrawGhost(gs);
        DrawCurrentPiece(gs);
    }
    DrawHUD(gs);
    if (gs.gameOver)  DrawGameOver(gs);
    if (gs.paused)    DrawPauseOverlay();
}

// ── Main ──────────────────────────────────────────────────────────────────────
int main() {
    srand((unsigned)time(nullptr));

    InitWindow(WIN_W, WIN_H, "Tetris");
    SetTargetFPS(60);

    GameState gs;
    InitGame(gs);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        HandleInput(gs);

        // restart
        if (gs.gameOver && IsKeyPressed(KEY_R))
            InitGame(gs);

        if (!gs.gameOver && !gs.paused)
            UpdateGame(gs, dt);

        BeginDrawing();
            RenderFrame(gs);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

# ura-tcp-server

A TCP chat server and client implemented in [Ura](https://github.com/mohammedhrima/ura-lang) — a compiled, statically-typed language with Python-like syntax.

## Getting the compiler

```bash
curl -L https://github.com/mohammedhrima/ura-lang/raw/main/build/ura -o ura
chmod +x ura
```

## Usage

```bash
./ura <file.ura>
```

## Running the server and client

Open two terminals:

**Terminal 1 — start the server:**
```bash
./ura basic/server.ura
# Listens on port 17000
```

**Terminal 2 — connect a client:**
```bash
./ura basic/client.ura
# Connects to localhost:17000
```

Type messages in either terminal and they will be echoed to the other side.

## Project structure

```
basic/
  server.ura    TCP server (accepts one client, bidirectional messaging)
  client.ura    TCP client
cmd/
  server.ura    Command-based server variant
  client.ura    Command-based client variant
utils.ura       Shared networking utilities (SockAddr, logging helpers)
```

## About Ura

Ura is a compiled language that targets native code via LLVM. Source: [github.com/mohammedhrima/ura-lang](https://github.com/mohammedhrima/ura-lang)

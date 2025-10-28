; ModuleID = '/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura'
source_filename = "/Users/hrimamohammed/Desktop/Personal/ura-lang/src/file.ura"

@str_literal = private constant [2 x i8] c"b\00"
@str_literal.1 = private constant [2 x i8] c"a\00"

define i32 @to_ascci(i8 %c) {
entry:
  %EQUAL = icmp eq i8 %c, 97
  br i1 %EQUAL, label %if, label %end_if

if:                                               ; preds = %entry
  ret i32 97
  br label %end_if

end_if:                                           ; preds = %if, %entry
  %EQUAL1 = icmp eq i8 %c, 98
  br i1 %EQUAL1, label %if2, label %end_if3

if2:                                              ; preds = %end_if
  ret i32 98
  br label %end_if3

end_if3:                                          ; preds = %if2, %end_if
  %EQUAL4 = icmp eq i8 %c, 99
  br i1 %EQUAL4, label %if5, label %end_if6

if5:                                              ; preds = %end_if3
  ret i32 99
  br label %end_if6

end_if6:                                          ; preds = %if5, %end_if3
  %EQUAL7 = icmp eq i8 %c, 100
  br i1 %EQUAL7, label %if8, label %end_if9

if8:                                              ; preds = %end_if6
  ret i32 100
  br label %end_if9

end_if9:                                          ; preds = %if8, %end_if6
  %EQUAL10 = icmp eq i8 %c, 101
  br i1 %EQUAL10, label %if11, label %end_if12

if11:                                             ; preds = %end_if9
  ret i32 101
  br label %end_if12

end_if12:                                         ; preds = %if11, %end_if9
  %EQUAL13 = icmp eq i8 %c, 102
  br i1 %EQUAL13, label %if14, label %end_if15

if14:                                             ; preds = %end_if12
  ret i32 102
  br label %end_if15

end_if15:                                         ; preds = %if14, %end_if12
  %EQUAL16 = icmp eq i8 %c, 103
  br i1 %EQUAL16, label %if17, label %end_if18

if17:                                             ; preds = %end_if15
  ret i32 103
  br label %end_if18

end_if18:                                         ; preds = %if17, %end_if15
  %EQUAL19 = icmp eq i8 %c, 104
  br i1 %EQUAL19, label %if20, label %end_if21

if20:                                             ; preds = %end_if18
  ret i32 104
  br label %end_if21

end_if21:                                         ; preds = %if20, %end_if18
  %EQUAL22 = icmp eq i8 %c, 105
  br i1 %EQUAL22, label %if23, label %end_if24

if23:                                             ; preds = %end_if21
  ret i32 105
  br label %end_if24

end_if24:                                         ; preds = %if23, %end_if21
  %EQUAL25 = icmp eq i8 %c, 106
  br i1 %EQUAL25, label %if26, label %end_if27

if26:                                             ; preds = %end_if24
  ret i32 106
  br label %end_if27

end_if27:                                         ; preds = %if26, %end_if24
  %EQUAL28 = icmp eq i8 %c, 107
  br i1 %EQUAL28, label %if29, label %end_if30

if29:                                             ; preds = %end_if27
  ret i32 107
  br label %end_if30

end_if30:                                         ; preds = %if29, %end_if27
  %EQUAL31 = icmp eq i8 %c, 108
  br i1 %EQUAL31, label %if32, label %end_if33

if32:                                             ; preds = %end_if30
  ret i32 108
  br label %end_if33

end_if33:                                         ; preds = %if32, %end_if30
  %EQUAL34 = icmp eq i8 %c, 109
  br i1 %EQUAL34, label %if35, label %end_if36

if35:                                             ; preds = %end_if33
  ret i32 109
  br label %end_if36

end_if36:                                         ; preds = %if35, %end_if33
  %EQUAL37 = icmp eq i8 %c, 110
  br i1 %EQUAL37, label %if38, label %end_if39

if38:                                             ; preds = %end_if36
  ret i32 110
  br label %end_if39

end_if39:                                         ; preds = %if38, %end_if36
  %EQUAL40 = icmp eq i8 %c, 111
  br i1 %EQUAL40, label %if41, label %end_if42

if41:                                             ; preds = %end_if39
  ret i32 111
  br label %end_if42

end_if42:                                         ; preds = %if41, %end_if39
  %EQUAL43 = icmp eq i8 %c, 112
  br i1 %EQUAL43, label %if44, label %end_if45

if44:                                             ; preds = %end_if42
  ret i32 112
  br label %end_if45

end_if45:                                         ; preds = %if44, %end_if42
  %EQUAL46 = icmp eq i8 %c, 113
  br i1 %EQUAL46, label %if47, label %end_if48

if47:                                             ; preds = %end_if45
  ret i32 113
  br label %end_if48

end_if48:                                         ; preds = %if47, %end_if45
  %EQUAL49 = icmp eq i8 %c, 114
  br i1 %EQUAL49, label %if50, label %end_if51

if50:                                             ; preds = %end_if48
  ret i32 114
  br label %end_if51

end_if51:                                         ; preds = %if50, %end_if48
  %EQUAL52 = icmp eq i8 %c, 115
  br i1 %EQUAL52, label %if53, label %end_if54

if53:                                             ; preds = %end_if51
  ret i32 115
  br label %end_if54

end_if54:                                         ; preds = %if53, %end_if51
  %EQUAL55 = icmp eq i8 %c, 116
  br i1 %EQUAL55, label %if56, label %end_if57

if56:                                             ; preds = %end_if54
  ret i32 116
  br label %end_if57

end_if57:                                         ; preds = %if56, %end_if54
  %EQUAL58 = icmp eq i8 %c, 117
  br i1 %EQUAL58, label %if59, label %end_if60

if59:                                             ; preds = %end_if57
  ret i32 117
  br label %end_if60

end_if60:                                         ; preds = %if59, %end_if57
  %EQUAL61 = icmp eq i8 %c, 118
  br i1 %EQUAL61, label %if62, label %end_if63

if62:                                             ; preds = %end_if60
  ret i32 118
  br label %end_if63

end_if63:                                         ; preds = %if62, %end_if60
  %EQUAL64 = icmp eq i8 %c, 119
  br i1 %EQUAL64, label %if65, label %end_if66

if65:                                             ; preds = %end_if63
  ret i32 119
  br label %end_if66

end_if66:                                         ; preds = %if65, %end_if63
  %EQUAL67 = icmp eq i8 %c, 120
  br i1 %EQUAL67, label %if68, label %end_if69

if68:                                             ; preds = %end_if66
  ret i32 120
  br label %end_if69

end_if69:                                         ; preds = %if68, %end_if66
  %EQUAL70 = icmp eq i8 %c, 121
  br i1 %EQUAL70, label %if71, label %end_if72

if71:                                             ; preds = %end_if69
  ret i32 121
  br label %end_if72

end_if72:                                         ; preds = %if71, %end_if69
  %EQUAL73 = icmp eq i8 %c, 122
  br i1 %EQUAL73, label %if74, label %end_if75

if74:                                             ; preds = %end_if72
  ret i32 122
  br label %end_if75

end_if75:                                         ; preds = %if74, %end_if72
  %EQUAL76 = icmp eq i8 %c, 65
  br i1 %EQUAL76, label %if77, label %end_if78

if77:                                             ; preds = %end_if75
  ret i32 65
  br label %end_if78

end_if78:                                         ; preds = %if77, %end_if75
  %EQUAL79 = icmp eq i8 %c, 66
  br i1 %EQUAL79, label %if80, label %end_if81

if80:                                             ; preds = %end_if78
  ret i32 66
  br label %end_if81

end_if81:                                         ; preds = %if80, %end_if78
  %EQUAL82 = icmp eq i8 %c, 67
  br i1 %EQUAL82, label %if83, label %end_if84

if83:                                             ; preds = %end_if81
  ret i32 67
  br label %end_if84

end_if84:                                         ; preds = %if83, %end_if81
  %EQUAL85 = icmp eq i8 %c, 68
  br i1 %EQUAL85, label %if86, label %end_if87

if86:                                             ; preds = %end_if84
  ret i32 68
  br label %end_if87

end_if87:                                         ; preds = %if86, %end_if84
  %EQUAL88 = icmp eq i8 %c, 69
  br i1 %EQUAL88, label %if89, label %end_if90

if89:                                             ; preds = %end_if87
  ret i32 69
  br label %end_if90

end_if90:                                         ; preds = %if89, %end_if87
  %EQUAL91 = icmp eq i8 %c, 70
  br i1 %EQUAL91, label %if92, label %end_if93

if92:                                             ; preds = %end_if90
  ret i32 70
  br label %end_if93

end_if93:                                         ; preds = %if92, %end_if90
  %EQUAL94 = icmp eq i8 %c, 71
  br i1 %EQUAL94, label %if95, label %end_if96

if95:                                             ; preds = %end_if93
  ret i32 71
  br label %end_if96

end_if96:                                         ; preds = %if95, %end_if93
  %EQUAL97 = icmp eq i8 %c, 72
  br i1 %EQUAL97, label %if98, label %end_if99

if98:                                             ; preds = %end_if96
  ret i32 72
  br label %end_if99

end_if99:                                         ; preds = %if98, %end_if96
  %EQUAL100 = icmp eq i8 %c, 73
  br i1 %EQUAL100, label %if101, label %end_if102

if101:                                            ; preds = %end_if99
  ret i32 73
  br label %end_if102

end_if102:                                        ; preds = %if101, %end_if99
  %EQUAL103 = icmp eq i8 %c, 74
  br i1 %EQUAL103, label %if104, label %end_if105

if104:                                            ; preds = %end_if102
  ret i32 74
  br label %end_if105

end_if105:                                        ; preds = %if104, %end_if102
  %EQUAL106 = icmp eq i8 %c, 75
  br i1 %EQUAL106, label %if107, label %end_if108

if107:                                            ; preds = %end_if105
  ret i32 75
  br label %end_if108

end_if108:                                        ; preds = %if107, %end_if105
  %EQUAL109 = icmp eq i8 %c, 76
  br i1 %EQUAL109, label %if110, label %end_if111

if110:                                            ; preds = %end_if108
  ret i32 76
  br label %end_if111

end_if111:                                        ; preds = %if110, %end_if108
  %EQUAL112 = icmp eq i8 %c, 77
  br i1 %EQUAL112, label %if113, label %end_if114

if113:                                            ; preds = %end_if111
  ret i32 77
  br label %end_if114

end_if114:                                        ; preds = %if113, %end_if111
  %EQUAL115 = icmp eq i8 %c, 78
  br i1 %EQUAL115, label %if116, label %end_if117

if116:                                            ; preds = %end_if114
  ret i32 78
  br label %end_if117

end_if117:                                        ; preds = %if116, %end_if114
  %EQUAL118 = icmp eq i8 %c, 79
  br i1 %EQUAL118, label %if119, label %end_if120

if119:                                            ; preds = %end_if117
  ret i32 79
  br label %end_if120

end_if120:                                        ; preds = %if119, %end_if117
  %EQUAL121 = icmp eq i8 %c, 80
  br i1 %EQUAL121, label %if122, label %end_if123

if122:                                            ; preds = %end_if120
  ret i32 80
  br label %end_if123

end_if123:                                        ; preds = %if122, %end_if120
  %EQUAL124 = icmp eq i8 %c, 81
  br i1 %EQUAL124, label %if125, label %end_if126

if125:                                            ; preds = %end_if123
  ret i32 81
  br label %end_if126

end_if126:                                        ; preds = %if125, %end_if123
  %EQUAL127 = icmp eq i8 %c, 82
  br i1 %EQUAL127, label %if128, label %end_if129

if128:                                            ; preds = %end_if126
  ret i32 82
  br label %end_if129

end_if129:                                        ; preds = %if128, %end_if126
  %EQUAL130 = icmp eq i8 %c, 83
  br i1 %EQUAL130, label %if131, label %end_if132

if131:                                            ; preds = %end_if129
  ret i32 83
  br label %end_if132

end_if132:                                        ; preds = %if131, %end_if129
  %EQUAL133 = icmp eq i8 %c, 84
  br i1 %EQUAL133, label %if134, label %end_if135

if134:                                            ; preds = %end_if132
  ret i32 84
  br label %end_if135

end_if135:                                        ; preds = %if134, %end_if132
  %EQUAL136 = icmp eq i8 %c, 85
  br i1 %EQUAL136, label %if137, label %end_if138

if137:                                            ; preds = %end_if135
  ret i32 85
  br label %end_if138

end_if138:                                        ; preds = %if137, %end_if135
  %EQUAL139 = icmp eq i8 %c, 86
  br i1 %EQUAL139, label %if140, label %end_if141

if140:                                            ; preds = %end_if138
  ret i32 86
  br label %end_if141

end_if141:                                        ; preds = %if140, %end_if138
  %EQUAL142 = icmp eq i8 %c, 87
  br i1 %EQUAL142, label %if143, label %end_if144

if143:                                            ; preds = %end_if141
  ret i32 87
  br label %end_if144

end_if144:                                        ; preds = %if143, %end_if141
  %EQUAL145 = icmp eq i8 %c, 88
  br i1 %EQUAL145, label %if146, label %end_if147

if146:                                            ; preds = %end_if144
  ret i32 88
  br label %end_if147

end_if147:                                        ; preds = %if146, %end_if144
  %EQUAL148 = icmp eq i8 %c, 89
  br i1 %EQUAL148, label %if149, label %end_if150

if149:                                            ; preds = %end_if147
  ret i32 89
  br label %end_if150

end_if150:                                        ; preds = %if149, %end_if147
  %EQUAL151 = icmp eq i8 %c, 90
  br i1 %EQUAL151, label %if152, label %end_if153

if152:                                            ; preds = %end_if150
  ret i32 90
  br label %end_if153

end_if153:                                        ; preds = %if152, %end_if150
  %EQUAL154 = icmp eq i8 %c, 48
  br i1 %EQUAL154, label %if155, label %end_if156

if155:                                            ; preds = %end_if153
  ret i32 48
  br label %end_if156

end_if156:                                        ; preds = %if155, %end_if153
  %EQUAL157 = icmp eq i8 %c, 49
  br i1 %EQUAL157, label %if158, label %end_if159

if158:                                            ; preds = %end_if156
  ret i32 49
  br label %end_if159

end_if159:                                        ; preds = %if158, %end_if156
  %EQUAL160 = icmp eq i8 %c, 50
  br i1 %EQUAL160, label %if161, label %end_if162

if161:                                            ; preds = %end_if159
  ret i32 50
  br label %end_if162

end_if162:                                        ; preds = %if161, %end_if159
  %EQUAL163 = icmp eq i8 %c, 51
  br i1 %EQUAL163, label %if164, label %end_if165

if164:                                            ; preds = %end_if162
  ret i32 51
  br label %end_if165

end_if165:                                        ; preds = %if164, %end_if162
  %EQUAL166 = icmp eq i8 %c, 52
  br i1 %EQUAL166, label %if167, label %end_if168

if167:                                            ; preds = %end_if165
  ret i32 52
  br label %end_if168

end_if168:                                        ; preds = %if167, %end_if165
  %EQUAL169 = icmp eq i8 %c, 53
  br i1 %EQUAL169, label %if170, label %end_if171

if170:                                            ; preds = %end_if168
  ret i32 53
  br label %end_if171

end_if171:                                        ; preds = %if170, %end_if168
  %EQUAL172 = icmp eq i8 %c, 54
  br i1 %EQUAL172, label %if173, label %end_if174

if173:                                            ; preds = %end_if171
  ret i32 54
  br label %end_if174

end_if174:                                        ; preds = %if173, %end_if171
  %EQUAL175 = icmp eq i8 %c, 55
  br i1 %EQUAL175, label %if176, label %end_if177

if176:                                            ; preds = %end_if174
  ret i32 55
  br label %end_if177

end_if177:                                        ; preds = %if176, %end_if174
  %EQUAL178 = icmp eq i8 %c, 56
  br i1 %EQUAL178, label %if179, label %end_if180

if179:                                            ; preds = %end_if177
  ret i32 56
  br label %end_if180

end_if180:                                        ; preds = %if179, %end_if177
  %EQUAL181 = icmp eq i8 %c, 57
  br i1 %EQUAL181, label %if182, label %end_if183

if182:                                            ; preds = %end_if180
  ret i32 57
  br label %end_if183

end_if183:                                        ; preds = %if182, %end_if180
  %EQUAL184 = icmp eq i8 %c, 32
  br i1 %EQUAL184, label %if185, label %end_if186

if185:                                            ; preds = %end_if183
  ret i32 32
  br label %end_if186

end_if186:                                        ; preds = %if185, %end_if183
  %EQUAL187 = icmp eq i8 %c, 0
  br i1 %EQUAL187, label %if188, label %end_if189

if188:                                            ; preds = %end_if186
  ret i32 0
  br label %end_if189

end_if189:                                        ; preds = %if188, %end_if186
  ret i32 -1
}

define i32 @strcmp(ptr %left, ptr %right) {
entry:
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %while

while:                                            ; preds = %while_bloc, %entry
  %i1 = load i32, ptr %i, align 4
  %access = getelementptr i8, ptr %left, i32 %i1
  %access_val = load i8, ptr %access, align 1
  %i2 = load i32, ptr %i, align 4
  %access3 = getelementptr i8, ptr %right, i32 %i2
  %access_val4 = load i8, ptr %access3, align 1
  %EQUAL = icmp eq i8 %access_val, %access_val4
  %i5 = load i32, ptr %i, align 4
  %access6 = getelementptr i8, ptr %left, i32 %i5
  %access_val7 = load i8, ptr %access6, align 1
  %NOT_EQ = icmp ne i8 %access_val7, 0
  %AND = and i1 %EQUAL, %NOT_EQ
  br i1 %AND, label %while_bloc, label %end_while

while_bloc:                                       ; preds = %while
  %i8 = load i32, ptr %i, align 4
  %ADD = add i32 %i8, 1
  store i32 %ADD, ptr %i, align 4
  br label %while

end_while:                                        ; preds = %while
  %i9 = load i32, ptr %i, align 4
  %access10 = getelementptr i8, ptr %left, i32 %i9
  %access_val11 = load i8, ptr %access10, align 1
  %to_ascci = call i32 @to_ascci(i8 %access_val11)
  %i12 = load i32, ptr %i, align 4
  %access13 = getelementptr i8, ptr %right, i32 %i12
  %access_val14 = load i8, ptr %access13, align 1
  %to_ascci15 = call i32 @to_ascci(i8 %access_val14)
  %SUB = sub i32 %to_ascci, %to_ascci15
  ret i32 %SUB
}

define i32 @main() {
entry:
  %len = alloca i32, align 4
  %strcmp = call i32 @strcmp(ptr @str_literal, ptr @str_literal.1)
  store i32 %strcmp, ptr %len, align 4
  %len1 = load i32, ptr %len, align 4
  ret i32 %len1
}

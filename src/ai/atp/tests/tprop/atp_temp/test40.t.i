#;
argument {
A --> B;
B --> C;
C --> ~A;
A || B || C;
#;
SOS |- A;
}
#;
argument {
A --> B;
B --> C;
C --> ~A;
A && B && C;
#;
SOS |- B;
}
#;
quit;

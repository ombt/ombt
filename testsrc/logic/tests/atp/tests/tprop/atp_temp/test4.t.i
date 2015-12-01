#;
argument {
A --> (B --> C);
A --> B;
SOS |- A --> C;
}
#;
argument {
A --> (B --> C);
A --> C;
SOS |- A --> B;
}
#;
argument {
A --> B;
A --> C;
SOS |- A --> (B && C);
}
#;
argument {
A --> C;
B --> C;
SOS |- (A || B) --> C;
}
#;
argument {
B --> (A --> C);
SOS |- A --> (B --> C);
}
#;
argument {
A;
SOS |- (A --> B) --> B;
}
#;
argument {
A --> (A --> B);
SOS |- A --> B;
}
#;
argument {
(A --> B) --> A;
SOS |- A;
}
#;
argument {
(A --> B) --> C;
SOS |- (A --> C) --> C;
}
#;
argument {
(A --> C) --> B;
SOS |- (A --> B ) --> C;
}
#;
argument {
(A --> B) --> B;
SOS |- (B --> A) --> A;
}
#;
argument {
~(A --> C);
SOS |- A --> ~C;
}
#;
argument {
A --> ~C;
SOS |- ~(A --> C);
}
#;
argument {
A --> ~A;
SOS |- ~A;
}
#;
argument {
C --> A;
SOS |- ~(A --> C);
}
#;
argument {
~(A --> C);
SOS |- C --> A;
}
#;
argument {
A --> (B --> C);
SOS |- (A && B) --> C;
}
#;
argument {
(A && B) --> C;
SOS |- A --> (B --> C);
}
#;
argument {
(A || B) --> C;
SOS |- A --> C;
}
#;
argument {
A --> C;
SOS |- (A || B) --> C;
}
#;
quit;

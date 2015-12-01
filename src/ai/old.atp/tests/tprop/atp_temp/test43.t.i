#;
# PROBLEM 2.9.3, PAGE 94 IN LOGIC FOR MATHEMATICS AND COMPUTER SCIENCE;
#;
argument {
F --> G;
SOS |- (F && H) --> (G && H);
}
#;
argument {
F --> G;
SOS |- (F || H) --> (G || H);
}
#;
argument {
F --> G;
SOS |- (F --> H) --> (G --> H);
}
#;
argument {
F --> G;
SOS |- (F <--> H) --> (G <--> H);
}
#;
# PROBLEM 2.9.4, PAGE 95 IN LOGIC FOR MATHEMATICS AND COMPUTER SCIENCE;
#;
argument {
F --> G;
SOS |- (H && F) --> (H && G);
}
#;
argument {
F --> G;
SOS |- (H || F) --> (H || G);
}
#;
argument {
F --> G;
SOS |- (H --> F) --> (H --> G);
}
#;
argument {
F --> G;
SOS |- (H <--> F) --> (H <--> G);
}
#;
# PROBLEM 2.9.5, PAGE 94 IN LOGIC FOR MATHEMATICS AND COMPUTER SCIENCE;
#;
argument {
F <--> G;
SOS |- (F && H) <--> (G && H);
}
#;
argument {
F <--> G;
SOS |- (F || H) <--> (G || H);
}
#;
argument {
F <--> G;
SOS |- (F --> H) <--> (G --> H);
}
#;
argument {
F <--> G;
SOS |- (F <--> H) <--> (G <--> H);
}
#;
# PROBLEM 2.9.6, PAGE 95 IN LOGIC FOR MATHEMATICS AND COMPUTER SCIENCE;
#;
argument {
F <--> G;
SOS |- (H && F) <--> (H && G);
}
#;
argument {
F <--> G;
SOS |- (H || F) <--> (H || G);
}
#;
argument {
F <--> G;
SOS |- (H --> F) <--> (H --> G);
}
#;
argument {
F <--> G;
SOS |- (H <--> F) <--> (H <--> G);
}
#;
quit;

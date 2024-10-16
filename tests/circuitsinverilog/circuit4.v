module circuit4(
    input A,
    input B,
    output F
);
    wire w1;
    not (w1, A);    // w1 = NOT A
    xor (F, w1, B); // F = w1 XOR B
endmodule

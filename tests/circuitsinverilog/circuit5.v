module circuit5(
    input A,
    input B,
    input C,
    output F
);
    wire w1;
    and (w1, A, B, C); // w1 = A AND B AND C
    not (F, w1);       // F = NOT w1
endmodule

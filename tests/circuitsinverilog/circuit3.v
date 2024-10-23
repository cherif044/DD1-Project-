module circuit3(
    input A,
    input B,
    input C,
    output F
);
    wire w1;
    or (w1, A, B);  // w1 = A OR B
    and (F, w1, C); // F = w1 AND C
endmodule

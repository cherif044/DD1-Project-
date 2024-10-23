module circuit1(
    input A,
    input B,
    input C,
    output F
);
    wire w1;
    and (w1, B, C); // w1 = B AND C
    or (F, A, w1);  // F = A OR w1
endmodule

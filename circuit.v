module circuit1(
    input A,
    input B,
    input C,
    output F
);
    wire w1;
    and (w1, B, C); 
    or (F, A, w1);  
endmodule

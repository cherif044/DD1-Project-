module circuit1(
    input A,
    input B,
    input C,
    input D,
    output F
  );
    wire w1;
    wire w2;
    and (w1, B, C);
    and (w2, A, D)
    or (F, w1, w2);  
    
endmodule

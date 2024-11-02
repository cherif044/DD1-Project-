module complex_circuit(
    input A,
    input B,
    input C,
    input D,
    output F,
    output G
);
    wire w1, w2, w3;

    
    xor (w1, A, B);        
    nand (w2, B, C);        
    xor (w3, w1, D);       

   
    nand (F, w2, w3);    
    xor (G, w1, w2);     
endmodule

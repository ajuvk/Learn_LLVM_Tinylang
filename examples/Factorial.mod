MODULE Fact;

PROCEDURE FACT(N:INTEGER):INTEGER;

VAR K: INTEGER;
VAR I: INTEGER;
BEGIN
    IF N <= 1 THEN
    	RETURN 1;
    END;
    
    K := 1;
    I := 1;
    WHILE (I <= N)  DO
    	K := K*I;
    	I := I + 1;
   END;
   RETURN K;
END FACT;

END Fact.

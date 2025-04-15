type  
    natural = 0..maxint;  
    count = integer;  
    range = integer;  
    angle = real;  
    color = (red, yellow, green, blue);  
    sex = (male, female);  
    year = 1900..1999;  
    shape = (triangle, rectangle, circle);  
    punchedcard = array [1..80] of char;  
    charsequence = file of char;  
    polar = record  
        r : real;  
        theta : angle  
    end;  
    indextype = 1..limit;  
    vector = array [indextype] of real;
    person = record  
        s : sex;  
        a : integer;
        i : real  
    end;  

const  
    eps = 1e-10;

var  
    x, y, z, max : real;  
    i, j : integer;  
    k : 0..9;  
    p, q, r : Boolean;  
    operator : (plus, minus, times);  
    a : array [0..63] of real;  
    c : color;  
    f : file of char;  
    hue1, hue2 : set of color;  
    p1, p2 : person;
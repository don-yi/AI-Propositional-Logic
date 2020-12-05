#include "pl.h"
#include <iostream>

#define xstr(s) str(s)
#define str(s) #s
#define var(X) Literal X( xstr(X) )
#define var2(X) Literal ( xstr(X) )


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void test0() {
    var(A); var(B); var(C); var(D); var(E);
    ////////////////////////////////////////////////////////////////////////////
    KnowledgeBase kb;
    kb += A>B;
    kb += B>C;
    CNF alpha = A>C;

    //std::cout << "Does " << kb << " imply " << alpha << "?" << std::endl;

    if ( kb.ProveByRefutation( alpha ) ) { std::cout << "Proved" << std::endl; } 
    else                                 { std::cout << "Not proved" << std::endl; }
}

void test1() {
    var(A); var(B); var(C); var(D); var(E);
    ////////////////////////////////////////////////////////////////////////////
    KnowledgeBase kb;
    kb += A>(~B&~C);
    kb += ~D>B;
    kb +=  E>C;
    CNF alpha = ~A | (D&~E);

    //std::cout << "Does " << kb << " imply " << alpha << "?" << std::endl;

    if ( kb.ProveByRefutation( alpha ) ) { std::cout << "Proved" << std::endl; } 
    else                                 { std::cout << "Not proved" << std::endl; }
}

void test2() {
    var(A); var(B); var(C); var(D); var(E);
    ////////////////////////////////////////////////////////////////////////////
    KnowledgeBase kb;
    kb += D>(A&~C);
    kb += A>E;
    kb += ~D>~B;
    CNF alpha = ~E > (~B|~C);

    //std::cout << "Does " << kb << " imply " << alpha << "?" << std::endl;

    if ( kb.ProveByRefutation( alpha ) ) { std::cout << "Proved" << std::endl; } 
    else                                 { std::cout << "Not proved" << std::endl; }
}

////////////////////////////////////////////////////////////////////////////////
void test3() { 
    var(A); var(B); var(C); var(D);
    KnowledgeBase kb;
    kb += (A|B)>C;
    kb += D>(A|C);
    kb += ~A>D;
    CNF alpha = ~C > (A&~B);

    //std::cout << "Does " << kb << " imply " << alpha << "?" << std::endl;

    if ( kb.ProveByRefutation( alpha ) ) { std::cout << "Proved" << std::endl; } 
    else                                 { std::cout << "Not proved" << std::endl; }
}

////////////////////////////////////////////////////////////////////////////////
void test4() { 
    // check for elimination of 2 pairs of complimentary symbols
    var(A); var(B); var(C);
    KnowledgeBase kb;
    kb += A|B;
    kb += ~A|~B;
    CNF alpha = A;

    std::cout << "Checking for elimination of 2 pairs of complimentary symbols, should be \"Not proved\":\n";

    if ( kb.ProveByRefutation( alpha ) ) { std::cout << "Proved" << std::endl; } 
    else                                 { std::cout << "Not proved" << std::endl; }

    std::cout << "Done checking for elimination of 2 pairs of complimentary symbols" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
void test5() {
    // incorrect optimization that only considers pairs if one of the sentences is a single literal
    // Example: A|B|C resolve ~A
    var(A); var(B); var(C); var(D); var(E);
    ////////////////////////////////////////////////////////////////////////////
    KnowledgeBase kb;
    kb +=  A |  B ;
    kb +=  A | ~B ;
    kb += ~A |  C ;
    CNF alpha = A&C ;

    //std::cout << "Does " << kb << " imply " << alpha << "?" << std::endl;

    std::cout << "Checking incorrect optimization (should be \"Proved\"):\n";
    
    if ( kb.ProveByRefutation( alpha ) ) { std::cout << "Proved" << std::endl; } 
    else                                 { std::cout << "Not proved" << std::endl; }
    
    std::cout << "Done checking incorrect optimization\n";
}

////////////////////////////////////////////////////////////////////////////////
void test6() { // prove tautology
    var(A); var(B); var(C);
    KnowledgeBase kb;
    CNF alpha = A | ~A;

    //std::cout << "Does " << kb << " imply " << alpha << "?" << std::endl;

#ifndef BITSTRING
    if ( kb.ProveByRefutation( alpha ) ) { std::cout << "Proved" << std::endl; } 
    else                                 { std::cout << "Not proved" << std::endl; }
#endif
}

////////////////////////////////////////////////////////////////////////////////
void test7() { // simple "cannot prove test"
    var(A); var(B); var(C);
    KnowledgeBase kb;
    kb += A;
    CNF alpha = ~A;

    //std::cout << "Does " << kb << " imply " << alpha << "?" << std::endl;

    if ( kb.ProveByRefutation( alpha ) ) { std::cout << "Proved" << std::endl; } 
    else                                 { std::cout << "Not proved" << std::endl; }
}

////////////////////////////////////////////////////////////////////////////////
void test8() { 
    test4();
    test5();

    var(A1); var(A2); var(A3); var(A4); var(A5); var(A6); 
    var(B1); var(B2); var(B3); var(B4); var(B5); var(B6); 
    var(C1); var(C2); var(C3); var(C4); var(C5); var(C6); 
    var(D1); var(D2); var(D3); var(D4); var(D5); var(D6);
    var(E1); var(E2); var(E3); var(E4); var(E5); var(E6);
    KnowledgeBase kb;
    kb += (A1 | B1 ) > C2;
    kb += (A2 | B2 ) > C3;
    kb += (A3 | B3 ) > C4;
    kb += (A4 | B4 ) > C5;
    kb += (A5 | B5 ) > C6;
    kb += ~A2 > (D1 | E1);
    kb += ~A3 > (D2 | E2);
    kb += ~A4 > (D3 | E3);
    kb += ~A5 > (D4 | E4);
    kb += ~A5 > (D4 | E4);
    kb += E1 > E2;
    kb += E2 > E3;
    kb += E3 > E4;
    kb += E4 > E5;
    kb += E5 > E6;
    kb += ~B2 > ~D1;
    kb += ~B3 > ~D2;
    kb += ~B4 > ~D3;
    kb += ~B5 > ~D4;
    kb += ~B6 > ~D5;
    CNF alpha = ~C5 > E4;

    //std::cout << "Does " << kb << " imply " << alpha << "?" << std::endl;

    if ( kb.ProveByRefutation( alpha ) ) { std::cout << "Proved" << std::endl; } 
    else                                 { std::cout << "Not proved" << std::endl; }
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// tests 1-3 same problem, but different number of rules are inserted
// watch for exponential growth

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void test9() { //only USEFUL rules are inserted
    test4();
    test5();

    Literal S[4][4] = {
        {var2(S00),var2(S01),var2(S02),var2(S03)},
        {var2(S10),var2(S11),var2(S12),var2(S13)},
        {var2(S20),var2(S21),var2(S22),var2(S23)},
        {var2(S30),var2(S31),var2(S32),var2(S33)}
    };
    Literal W[4][4] = {
        {var2(W00),var2(W01),var2(W02),var2(W03)},
        {var2(W10),var2(W11),var2(W12),var2(W13)},
        {var2(W20),var2(W21),var2(W22),var2(W23)},
        {var2(W30),var2(W31),var2(W32),var2(W33)}
    };
    Literal Vis[4][4] = {
        {var2(Vis00),var2(Vis01),var2(Vis02),var2(Vis03)},
        {var2(Vis10),var2(Vis11),var2(Vis12),var2(Vis13)},
        {var2(Vis20),var2(Vis21),var2(Vis22),var2(Vis23)},
        {var2(Vis30),var2(Vis31),var2(Vis32),var2(Vis33)}
    };

    KnowledgeBase kb;
    //       S(x,y) => W(x-1,y)V W(x+1,y)V W(x,y-1)V W(x,y+1)
    kb += ( (S[1][2])> W[0][2] | W[2][2] | W[1][1] | W[1][3] );

    //    W(x,y) =>   S(x-1,y)^ S(x+1,y)^ S(x,y-1)^ S(x,y+1)
    kb += W[0][2] > (           S[1][2] & S[0][1] & S[0][3] );
    kb += W[2][2] > ( S[1][2] & S[3][2] & S[2][1] & S[2][3] );

    //not needed
    //kb += W[1][2] > ( S[0][2] & S[2][2] & S[1][1] & S[1][3] );

    //Vis(x,y) => ~W(x,y) ^ ~P(x,y)
    kb +=  Vis[0][1] > ~W[0][1];
    kb +=  Vis[1][1] > ~W[1][1];
    kb +=  Vis[2][1] > ~W[2][1];
    kb +=  Vis[1][2] > ~W[1][2];

    //  ^ Y
    //  |
    //  +-------------------+
    //3 |    |    |    |    |
    //2 |    | SB |    |    |
    //1 |B ~S|~B~S|B~S |    |
    //0 |    |    |    |    |
    //  +-------------------+--> X
    //    0    1    2    3
    kb += (Vis[0][1] & Vis[1][1] & Vis[2][1] & Vis[1][2] & ~S[0][1] & ~S[1][1] & ~S[2][1] & S[1][2]);
    CNF alpha( W[1][3] );
    //std::cout << "Does " << kb << " imply " << alpha << "?" << std::endl;

    if ( kb.ProveByRefutation( alpha ) ) { std::cout << "Proved" << std::endl; } 
    else                                 { std::cout << "Not proved" << std::endl; }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void test10() { // a little more rules are inserted
    test4();
    test5();

    Literal S[4][4] = {
        {var2(S00),var2(S01),var2(S02),var2(S03)},
        {var2(S10),var2(S11),var2(S12),var2(S13)},
        {var2(S20),var2(S21),var2(S22),var2(S23)},
        {var2(S30),var2(S31),var2(S32),var2(S33)}
    };
    Literal W[4][4] = {
        {var2(W00),var2(W01),var2(W02),var2(W03)},
        {var2(W10),var2(W11),var2(W12),var2(W13)},
        {var2(W20),var2(W21),var2(W22),var2(W23)},
        {var2(W30),var2(W31),var2(W32),var2(W33)}
    };
    Literal Vis[4][4] = {
        {var2(Vis00),var2(Vis01),var2(Vis02),var2(Vis03)},
        {var2(Vis10),var2(Vis11),var2(Vis12),var2(Vis13)},
        {var2(Vis20),var2(Vis21),var2(Vis22),var2(Vis23)},
        {var2(Vis30),var2(Vis31),var2(Vis32),var2(Vis33)}
    };

    KnowledgeBase kb;

    //S(x,y) => W(x+1,y) V W(x-1,y) V W(x,y+1) V W(x,y-1)
    for ( int x=0; x<4; ++x ) { 
        for ( int y=0; y<4; ++y ) {
            if ( (x!=1 || y!=2) && (x!=0 || y!=2) && (x!=2 || y!=2) && (x!=1 || y!=1) ) continue;
            kb += ( (S[x][y])>(CNF() | (x>0  ? W[x-1][y] : CNF()) | (x<4-1? W[x+1][y] : CNF()) | (y>0  ? W[x][y-1] : CNF()) | (y<4-1? W[x][y+1] : CNF())) );
            //       S(x,y) =>                 W(x-1,y)           V         W(x+1,y)           V         W(x,y-1)           V         W(x,y+1)
        } 
    }
    //W(x,y) => S(x+1,y) ^ S(x-1,y) ^ S(x,y+1) ^ S(x,y-1)
    for ( int x=0; x<4; ++x ) { 
        for ( int y=0; y<4; ++y ) {
            if ( (x!=1 || y!=2) && (x!=0 || y!=2) && (x!=2 || y!=2) && (x!=1 || y!=1) ) continue;
            kb += ( (W[x][y])>(CNF() & (x>0  ? S[x-1][y] : CNF()) & (x<4-1? S[x+1][y] : CNF()) & (y>0  ? S[x][y-1] : CNF()) & (y<4-1? S[x][y+1] : CNF())) );
            //       W(x,y) =>                 S(x-1,y)           ^         S(x+1,y)           ^         S(x,y-1)           ^         S(x,y+1)
        } 
    }

    //Vis(x,y) => ~W(x,y) ^ ~P(x,y)
    kb += Vis[0][1] > ~W[0][1];
    kb += Vis[1][1] > ~W[1][1];
    kb += Vis[2][1] > ~W[2][1];
    kb += Vis[1][2] > ~W[1][2];

    //  ^ Y
    //  |
    //  +-------------------+
    //3 |    |    |    |    |
    //2 |    | SB |    |    |
    //1 |B ~S|~B~S|B~S |    |
    //0 |    |    |    |    |
    //  +-------------------+--> X
    //    0    1    2    3
    kb += (Vis[0][1] & Vis[1][1] & Vis[2][1] & Vis[1][2] & ~S[0][1] & ~S[1][1] & ~S[2][1] & S[1][2]);
    CNF alpha( W[1][3] );
    //std::cout << "Does " << kb << " imply " << alpha << "?" << std::endl;

    if ( kb.ProveByRefutation( alpha ) ) { std::cout << "Proved" << std::endl; } 
    else                                 { std::cout << "Not proved" << std::endl; }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void test11() { // all rules are inserted
    test4();
    test5();

    int const X = 4;
    int const Y = 4;
    Literal S[X][Y] = {
        {var2(S00),var2(S01),var2(S02),var2(S03)},
        {var2(S10),var2(S11),var2(S12),var2(S13)},
        {var2(S20),var2(S21),var2(S22),var2(S23)},
        {var2(S30),var2(S31),var2(S32),var2(S33)}
    };
    Literal P[X][Y] = {
        {var2(P00),var2(P01),var2(P02),var2(P03)},
        {var2(P10),var2(P11),var2(P12),var2(P13)},
        {var2(P20),var2(P21),var2(P22),var2(P23)},
        {var2(P30),var2(P31),var2(P32),var2(P33)}
    };
    Literal B[X][Y] = {
        {var2(B00),var2(B01),var2(B02),var2(B03)},
        {var2(B10),var2(B11),var2(B12),var2(B13)},
        {var2(B20),var2(B21),var2(B22),var2(B23)},
        {var2(B30),var2(B31),var2(B32),var2(B33)}
    };
    Literal W[X][Y] = {
        {var2(W00),var2(W01),var2(W02),var2(W03)},
        {var2(W10),var2(W11),var2(W12),var2(W13)},
        {var2(W20),var2(W21),var2(W22),var2(W23)},
        {var2(W30),var2(W31),var2(W32),var2(W33)}
    };
    Literal Vis[X][Y] = {
        {var2(Vis00),var2(Vis01),var2(Vis02),var2(Vis03)},
        {var2(Vis10),var2(Vis11),var2(Vis12),var2(Vis13)},
        {var2(Vis20),var2(Vis21),var2(Vis22),var2(Vis23)},
        {var2(Vis30),var2(Vis31),var2(Vis32),var2(Vis33)}
    };

    KnowledgeBase kb;

    //S(x,y) => W(x+1,y) V W(x-1,y) V W(x,y+1) V W(x,y-1)
    for ( int x=0; x<X; ++x ) { 
        for ( int y=0; y<Y; ++y ) {

            //       S(x,y) =>                 W(x-1,y)           V         W(x+1,y)           V         W(x,y-1)           V         W(x,y+1)
            kb += ( (S[x][y])>(CNF() | (x>0  ? W[x-1][y] : CNF()) | (x<X-1? W[x+1][y] : CNF()) | (y>0  ? W[x][y-1] : CNF()) | (y<Y-1? W[x][y+1] : CNF())) );

            //       W(x,y) =>                 S(x-1,y)           ^         S(x+1,y)           ^         S(x,y-1)           ^         S(x,y+1)
            kb += ( (W[x][y])>(CNF() & (x>0  ? S[x-1][y] : CNF()) & (x<X-1? S[x+1][y] : CNF()) & (y>0  ? S[x][y-1] : CNF()) & (y<Y-1? S[x][y+1] : CNF())) );

            //       B(x,y) =>                 P(x-1,y)           V         P(x+1,y)           V         P(x,y-1)           V         P(x,y+1)
            kb += ( (B[x][y])>(CNF() | (x>0  ? P[x-1][y] : CNF()) | (x<X-1? P[x+1][y] : CNF()) | (y>0  ? P[x][y-1] : CNF()) | (y<Y-1? P[x][y+1] : CNF())) );

            //       P(x,y) =>                 B(x-1,y)           ^         B(x+1,y)           ^         B(x,y-1)           ^         B(x,y+1)
            kb += ( (P[x][y])>(CNF() & (x>0  ? B[x-1][y] : CNF()) & (x<X-1? B[x+1][y] : CNF()) & (y>0  ? B[x][y-1] : CNF()) & (y<Y-1? B[x][y+1] : CNF())) );

            //      Vis(x,y) =>  ~W(x,y)  ^ ~P(x,y)
            kb += ( Vis[x][y] > (~W[x][y] & ~P[x][y]) );
        } 
    }


    //  ^ Y
    //  |
    //  +-------------------+
    //3 |    |    |    |    |
    //2 |    | SB |    |    |
    //1 |B ~S|~B~S|B~S |    |
    //0 |    |    |    |    |
    //  +-------------------+--> X
    //    0    1    2    3
    kb += (Vis[0][1] & Vis[1][1] & Vis[2][1] & Vis[1][2] & B[0][1] & ~S[0][1] & ~B[1][1] & ~S[1][1] & B[2][1] & ~S[2][1] & B[1][2] & S[1][2]);
    CNF alpha( W[1][3] );
    //std::cout << "Does " << kb << " imply " << alpha << "?" << std::endl;

    if ( kb.ProveByRefutation( alpha ) ) { std::cout << "Proved" << std::endl; } 
    else                                 { std::cout << "Not proved" << std::endl; }
}

////////////////////////////////////////////////////////////////////////////////
void test12 () {
    test4();
    test5();

    int const X = 8;
    int const Y = 8;
    Literal S[X][Y] = {
        {var2(S00),var2(S01),var2(S02),var2(S03),  var2(S04),var2(S05),var2(S06),var2(S07)},
        {var2(S10),var2(S11),var2(S12),var2(S13),  var2(S14),var2(S15),var2(S16),var2(S17)},
        {var2(S20),var2(S21),var2(S22),var2(S23),  var2(S24),var2(S25),var2(S26),var2(S27)},
        {var2(S30),var2(S31),var2(S32),var2(S33),  var2(S34),var2(S35),var2(S36),var2(S37)},

        {var2(S40),var2(S41),var2(S42),var2(S43),  var2(S44),var2(S45),var2(S46),var2(S47)},
        {var2(S50),var2(S51),var2(S52),var2(S53),  var2(S54),var2(S55),var2(S56),var2(S57)},
        {var2(S60),var2(S61),var2(S62),var2(S63),  var2(S64),var2(S65),var2(S66),var2(S67)},
        {var2(S70),var2(S71),var2(S72),var2(S73),  var2(S74),var2(S75),var2(S76),var2(S77)},
    };
    //S[i][j] where both i and j are greater than 4 are not used

    KnowledgeBase kb;
    //at least 1 in each 4 block
    for ( int i=0; i<X/2; ++i ) { 
        kb += S[i][0] | S[i][1] | S[i][2] | S[i][3];
        kb += S[0][i] | S[1][i] | S[2][i] | S[3][i];

        kb += S[i][4] | S[i][5] | S[i][6] | S[i][7];
        kb += S[0][4+i] | S[1][4+i] | S[2][4+i] | S[3][4+i];

        kb += S[i+4][0] | S[i+4][1] | S[i+4][2] | S[i+4][3];
        kb += S[4][i] | S[5][i] | S[6][i] | S[7][i];
    }
    //only 1 in each line for all

    for ( int i=0; i<X/2; ++i ) { 
        for ( int j=0; j<Y/2; ++j ) { 
            kb += S[i][j] > ~S[i][(j+1)%4]; kb += S[i][j] > ~S[i][(j+2)%4]; kb += S[i][j] > ~S[i][(j+3)%4];
            kb += S[i][j] > ~S[(i+1)%4][j]; kb += S[i][j] > ~S[(i+2)%4][j]; kb += S[i][j] > ~S[(i+3)%4][j];

            kb += S[i][j+4] > ~S[i][(j+1)%4 +4];
            kb += S[i][j+4] > ~S[i][(j+2)%4 +4];
            kb += S[i][j+4] > ~S[i][(j+3)%4 +4];

            kb += S[i][j+4] > ~S[(i+1)%4 ][j+4];
            kb += S[i][j+4] > ~S[(i+2)%4 ][j+4];
            kb += S[i][j+4] > ~S[(i+3)%4 ][j+4];

            kb += S[i+4][j] > ~S[(i+1)%4 +4][j];
            kb += S[i+4][j] > ~S[(i+2)%4 +4][j];
            kb += S[i+4][j] > ~S[(i+3)%4 +4][j];

            kb += S[i+4][j] > ~S[i+4][(j+1)%4];
            kb += S[i+4][j] > ~S[i+4][(j+2)%4];
            kb += S[i+4][j] > ~S[i+4][(j+3)%4];
        }
    }

    //recital problem
    kb +=  S[1][5];
    kb += ~S[1][2];
    kb += ~S[5][2];

    kb += ~S[2][2];
    kb += ~S[3][2];
    kb += ~S[4][2];
    kb += ~S[7][2];

    kb += ~S[1][6];

    kb += ~S[4][3];

    kb += ~S[7][0];
    kb += ~S[7][3];

    kb += ~S[2][0];

    //CNF alpha( S[2][7] );
    CNF alpha( S[4][0] );
    //std::cout << "Does " << kb << " imply " << alpha << "?" << std::endl;

    if ( kb.ProveByRefutation( alpha ) ) { std::cout << "Proved" << std::endl; } 
    else                                 { std::cout << "Not proved" << std::endl; }
}

////////////////////////////////////////////////////////////////////////////////
void test13 () {
    test4();
    test5();

    int const X = 8;
    int const Y = 8;
    Literal S[X][Y] = {
        {var2(S00),var2(S01),var2(S02),var2(S03),  var2(S04),var2(S05),var2(S06),var2(S07)},
        {var2(S10),var2(S11),var2(S12),var2(S13),  var2(S14),var2(S15),var2(S16),var2(S17)},
        {var2(S20),var2(S21),var2(S22),var2(S23),  var2(S24),var2(S25),var2(S26),var2(S27)},
        {var2(S30),var2(S31),var2(S32),var2(S33),  var2(S34),var2(S35),var2(S36),var2(S37)},

        {var2(S40),var2(S41),var2(S42),var2(S43),  var2(S44),var2(S45),var2(S46),var2(S47)},
        {var2(S50),var2(S51),var2(S52),var2(S53),  var2(S54),var2(S55),var2(S56),var2(S57)},
        {var2(S60),var2(S61),var2(S62),var2(S63),  var2(S64),var2(S65),var2(S66),var2(S67)},
        {var2(S70),var2(S71),var2(S72),var2(S73),  var2(S74),var2(S75),var2(S76),var2(S77)},
    };
    //S[i][j] where both i and j are greater than 4 are not used

    KnowledgeBase kb;
    //at least 1 in each 4 block
    for ( int i=0; i<X/2; ++i ) { 
        kb += S[i][0] | S[i][1] | S[i][2] | S[i][3];
        kb += S[0][i] | S[1][i] | S[2][i] | S[3][i];

        kb += S[i][4] | S[i][5] | S[i][6] | S[i][7];
        kb += S[0][4+i] | S[1][4+i] | S[2][4+i] | S[3][4+i];

        kb += S[i+4][0] | S[i+4][1] | S[i+4][2] | S[i+4][3];
        kb += S[4][i] | S[5][i] | S[6][i] | S[7][i];
    }
    //only 1 in each line for all

    for ( int i=0; i<X/2; ++i ) { 
        for ( int j=0; j<Y/2; ++j ) { 
            kb += S[i][j] > ~S[i][(j+1)%4]; kb += S[i][j] > ~S[i][(j+2)%4]; kb += S[i][j] > ~S[i][(j+3)%4];
            kb += S[i][j] > ~S[(i+1)%4][j]; kb += S[i][j] > ~S[(i+2)%4][j]; kb += S[i][j] > ~S[(i+3)%4][j];

            kb += S[i][j+4] > ~S[i][(j+1)%4 +4];
            kb += S[i][j+4] > ~S[i][(j+2)%4 +4];
            kb += S[i][j+4] > ~S[i][(j+3)%4 +4];

            kb += S[i][j+4] > ~S[(i+1)%4 ][j+4];
            kb += S[i][j+4] > ~S[(i+2)%4 ][j+4];
            kb += S[i][j+4] > ~S[(i+3)%4 ][j+4];

            kb += S[i+4][j] > ~S[(i+1)%4 +4][j];
            kb += S[i+4][j] > ~S[(i+2)%4 +4][j];
            kb += S[i+4][j] > ~S[(i+3)%4 +4][j];

            kb += S[i+4][j] > ~S[i+4][(j+1)%4];
            kb += S[i+4][j] > ~S[i+4][(j+2)%4];
            kb += S[i+4][j] > ~S[i+4][(j+3)%4];
        }
    }

    //recital problem
    kb +=  S[1][5];
    kb += ~S[1][2];
    kb += ~S[5][2];

    kb += ~S[2][2];
    kb += ~S[3][2];
    kb += ~S[4][2];
    kb += ~S[7][2];

    kb += ~S[1][6];

    kb += ~S[4][3];

    kb += ~S[7][0];
    kb += ~S[7][3];

    kb += ~S[2][0];

    //CNF alpha( S[2][7] ); // cannot be proved !!!!
    CNF alpha( S[4][0] );
    //std::cout << "Does " << kb << " imply " << alpha << "?" << std::endl;

    if ( kb.ProveByRefutation( alpha ) ) { std::cout << "Proved" << std::endl; } 
    else                                 { std::cout << "Not proved" << std::endl; }
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void (*pTests[])(void) = { 
    test0, test1, test2, test3, test4, test5, test6, test7, test8, //should be immediate
    test9,  // 1 sec w/o optimizations
    test10, // 10 sec w/o optimizations
    // the rest take too long w/o optimizations
    test11, // less than 1 sec with bit-string 
    test12, // less than 1 sec with bit-string 
    test13, // less than 1 sec with bit-string and clean-up
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <cstdio> /* sscanf */
int main(int argc, char *argv[] ) {
    std::srand ( time (NULL) ) ;
    if (argc >1) {
        int test = 0;
        std::sscanf(argv[1],"%i",&test);
        try {
            pTests[test]();
        } catch( const char* msg) {
            std::cerr << msg << std::endl;
        }
    } else {
        //notes
        var(P); var(Q); var(S);
        CNF cnf = ~( (P>Q) > S  );
        std::cout << cnf << std::endl;
        // software ( ~P | ~S ) , ( ~P | Q ) , ( ~S ) , ( ~S | Q ) 
        // human    ( ~P | Q ) , ( ~S )
        // explanation - if all literals of S1 are in S2, S2 is strictly weaker and may be omitted
    }


    return 0;
}

////////// INIT ////////////////////////////////////////////////////////////////////////////////////
#include "CPluscal.hpp"
typedef vector<char>    vchar;
typedef vector<vchar>   vvchar;
typedef array<size_t,2> vec2i;



////////// CANVAS //////////////////////////////////////////////////////////////////////////////////

class TerminalCanvas{ public:
    // It's okay to reinvent the wheel if the wheel is very very small
    size_t Mrows;
    size_t Ncols;
    vvchar canvas;

    void init_canvas( size_t M, size_t N ){
        // Erase canvas
        Mrows = M;
        Ncols = N;
        vchar  emptyRow;

        emptyRow.reserve(N);
        for( size_t j = 0; j < N; ++j ){
            emptyRow.push_back(' ');
        }    

        canvas.clear();
        canvas.reserve(M);
        for( size_t i = 0; i < M; ++i ){  canvas.push_back( emptyRow );  }
    }

    string as_string(){
        // Get the contents of the canvas as a string
        string rtnStr = "";
        for( size_t i = 0; i < Mrows; ++i ){
            for( size_t j = 0; j < Ncols; ++j ){  rtnStr += canvas[i][j];  }   
            rtnStr += '\n';
        }
        return rtnStr;
    }

    void draw(){  cout << endl << as_string() << endl;  } // Render canvas

    void line( const vec2i& bgn, const vec2i& end ){

    }
};
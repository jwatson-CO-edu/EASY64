////////// INIT ////////////////////////////////////////////////////////////////////////////////////
///// Imports /////////////////////////////////////////////////////////////
#include "CPluscal.hpp"
typedef vector<char>    vchar;
typedef vector<vchar>   vvchar;
typedef array<size_t,2> vec2i;
typedef array<llong,2>  vec2l;
typedef array<double,2> vec2d;
typedef array<vec2i,2>  m2x2i;

///// Constants ///////////////////////////////////////////////////////////
const vec2d charAspect = {2.0,1.0};



////////// HELPER FUNCTIONS ////////////////////////////////////////////////////////////////////////

m2x2i bbox( const vec2i& bgn, const vec2i& end ){
    // Return a bounding box for the two end points
    return {
        vec2i{ min(bgn[0], end[0]), min(bgn[1], end[1]) },
        vec2i{ max(bgn[0], end[0]), max(bgn[1], end[1]) }
    };
}


deque<vec2l> bresenham_line( const llong& x0, const llong& y0, const llong& x1, const llong& y1 ){
    // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#Algorithm_for_integer_arithmetic
    llong /*--*/ dx = x1 - x0;
    llong /*--*/ dy = y1 - y0;
    llong /*--*/ D  = 2*dy - dx;
    llong /*--*/ y  = y0;
    deque<vec2l> rtnLin;
    for( llong x = x0; x <= x1; ++x ){
        rtnLin.push_back( {x, y} );  
        if( D > 0 ){
            ++y;
            D -= 2*dx;
        }
        D += 2*dy;
    }
    return rtnLin;
}



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
        vchar emptyRow;

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

    m2x2i line( const vec2i& bgn, const vec2i& end ){
        m2x2i /*--*/ rtnBox = bbox( bgn, end );
        deque<vec2l> coords = bresenham_line( (llong) bgn[0], (llong) bgn[1], (llong) end[0], (llong) end[1] );
    }

    m2x2i text_box( const vvstr& contents, const vec2i& upperLeft ){
        // PLEASE STOP
    }
};
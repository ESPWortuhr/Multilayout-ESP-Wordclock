#pragma once

class iUhrType
{

	public:
		unsigned int NUM_PIXELS;
		unsigned int NUM_SMATRIX;
		unsigned int ROWS_MATRIX;
		unsigned int NUM_RMATRIX;

    unsigned int matrix[][1]{};

    unsigned int smatrix[]{};

    unsigned int rmatrix[]{};

    unsigned int min_arr[][1]{};

    virtual void show(uint8_t text) const {};

    unsigned int getMatrix(uint8_t col, uint8_t row){
        return matrix[col][row];
    }

};

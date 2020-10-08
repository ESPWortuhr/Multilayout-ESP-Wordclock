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

	public:
    	virtual void show(uint8_t text) const {};

    	unsigned int getMatrix(const uint8_t &col,const uint8_t &row) const {
        return matrix[col][row];
    	}

    	unsigned int getSMatrix(const uint8_t &col) const {
			return smatrix[col];
    	}

		unsigned int getRMatrix(const uint8_t &col) const {
			return rmatrix[col];
		}

		unsigned int getMinArr(const uint8_t &col,const uint8_t &row) const {
			return min_arr[col][row];
    	}

};

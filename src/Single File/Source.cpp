#include <iostream>
#include <string>
#include <fstream>
#include "stdarg.h"
#include <vector>
#include <cstring>
#include "stdio.h"
#include <algorithm>


using namespace std;


class CMatrix {
	int nR, nC;
	double * * values;
public:
	CMatrix();
	~CMatrix();
	enum MI {
		MI_ZEROS,
		MI_ONES,
		MI_EYE,
		MI_RAND,
		MI_VALUE
	};
	CMatrix(int nR, int nC, int initialization = MI_ZEROS, double initializationValue = 0.0);
	CMatrix(int nR, int nC, double first, ...);
	CMatrix(const CMatrix & m);
	CMatrix(double d);
	CMatrix(string s);
	void copy(const CMatrix & m);
	void copy(double d);
	void copy(string s);
	void reset();
	string getString();

	CMatrix operator = (const CMatrix & m);
	CMatrix operator = (double d);
	CMatrix operator = (string s);
	void add(const CMatrix & m);
	void operator += (const CMatrix & m);
	void operator += (double d);
	CMatrix operator + (const CMatrix & m);
	CMatrix operator + (double d);
	void sub(const CMatrix & m);
	void operator -= (const CMatrix & m);
	void operator -= (double d);
	CMatrix operator - (const CMatrix & m);
	CMatrix operator - (double d);
	void mul(const CMatrix & m);
	void operator *= (const CMatrix & m);
	void operator *= (double d);
	CMatrix operator * (const CMatrix & m);
	CMatrix operator * (double d);
	/*void div(const CMatrix & m);
	void operator /= (const cmatrix & m);
	void operator /= (double d);
	cmatrix operator / (cmatrix & m);
	cmatrix operator / (double d);*/
	CMatrix operator++(); //Pre Increment
	CMatrix operator++(int); //Post Increment, int is not used
	CMatrix operator--(); //Pre Increment
	CMatrix operator--(int); //Post Increment, int is not used
	CMatrix operator - ();
	CMatrix operator + ();
	friend istream & operator >> (istream & is, CMatrix & C); //Stream
	friend ostream & operator << (ostream & os, CMatrix & C); //Stream
	void setSubMatrix(int iR, int iC, const CMatrix & m);
	CMatrix getSubMatrix(int r, int c, int nr, int nc);
	CMatrix getCofactor(int r, int c);
	void addColumn(const CMatrix & m);
	void addRow(const CMatrix & m);
	double & operator[](int i) {
		return values[i / nC][i % nC];
	}
	double & operator()(int i) {
		return values[i / nC][i % nC];
	}
	double & operator()(int r, int c) {
		return values[r][c];
	}
	int getn() {
		return nR * nC;
	};
	int getnR() {
		return nR;
	};
	int getnC() {
		return nC;
	};
	double getDeterminant();
	
	CMatrix getTranspose();
	
	CMatrix getInverse();
};





	CMatrix::CMatrix() {
		nR = nC = 0;
		values = NULL;
	}
	CMatrix::~CMatrix() {
			 reset();
		 }
	
	CMatrix::CMatrix(int nR, int nC, int initialization, double initializationValue) {
				  this -> nR = nR;
				  this -> nC = nC;
				  if((nR * nC) == 0) {
					  values = NULL;
					  return;
				  }
				  values = new double *[nR];
				  for (int iR = 0; iR < nR; iR++) {
					  values[iR] = new double[nC];
					  for (int iC = 0; iC < nC; iC++) {
						  switch (initialization) {
						  case MI_ZEROS :
	  values[iR][iC] = 0;
	  break;
						  case MI_ONES :
	  values[iR][iC] = 1;
	  break;
						  case MI_EYE :
	  values[iR][iC] = (iR == iC) ? 1 : 0;
	  break;
						  case MI_RAND :
	  values[iR][iC] = (rand() % 1000000) / 1000000.0;
	  break;
						  case MI_VALUE :
	  values[iR][iC] = initializationValue;
	  break;
						  }
					  }
				  }
			  }			   
	CMatrix ::CMatrix(int nR, int nC, double first, ...) {
		this -> nR = nR;
	   this -> nC = nC;
	   if((nR * nC) == 0) {
		   values = NULL;
		   return;
	   }
	   values = new double *[nR];
		   va_list va;

		   va_start(va, first);
   for (int iR = 0; iR < nR; iR++) {
	   values[iR] = new double[nC];
	   for (int iC = 0; iC < nC; iC++) {
		   values[iR][iC] = (iC == 0 && iR == 0) ? first : va_arg(va, double);
	   }
	}
	va_end(va);
		}
				   
	CMatrix ::CMatrix(const CMatrix & m) {
		nR = nC = 0;
		values = NULL;
		copy(m);
					   }
	CMatrix::CMatrix(string s) {
					   nR = nC = 0;
					   values = NULL;
					   copy(s);
				   }
	
	void CMatrix ::copy(const CMatrix & m) {
		reset();
		this -> nR = m.nR;
		this -> nC = m.nC;
		if((nR * nC) == 0) {
			values = NULL;
			return;
		}
		values = new double *[nR];
		for (int iR = 0; iR < nR; iR++) {
			values[iR] = new double[nC];
			for (int iC = 0; iC < nC; iC++) {
				values[iR][iC] = m.values[iR][iC];
			}
		}
	}

	CMatrix::CMatrix(double d) {
		nR = nC = 0;
		values = NULL;
		copy(d);
	}
	void CMatrix::copy(double d) {
		reset();
		this -> nR = 1;
		this -> nC = 1;
		values = new double *[1];
		values[0] = new double[1];
		values[0][0] = d;
	}

	void CMatrix ::copy(string s) {
		reset();
		char * buffer = new char[s.length() + 1];
		strcpy(buffer, s.c_str());
		char * lineContext;
		char * lineSeparators = ";\r\n";
		char * line = strtok(buffer, lineSeparators);
		while (line) {
			CMatrix row;
			char * context;
			char * separators = " []";
			char * token = strtok(line, separators);
			while (token) {
				CMatrix item = atof(token);
				row.addColumn(item);
				token = strtok(NULL, separators);
			}
			if(row.nC > 0 && (row.nC == nC || nR == 0))
				addRow(row);
			line = strtok(NULL, lineSeparators);
		}
		delete[] buffer;
	}

	void CMatrix ::reset() {
		if(values) {
			for (int i = 0; i < nR; i++)
				delete[] values[i];
			delete[] values;
		}
		nR = nC = 0;
		values = NULL;
	}
	string CMatrix ::getString() {
		string s;
		for (int iR = 0; iR < nR; iR++) {
			for (int iC = 0; iC < nC; iC++) {
				char buffer[50];
				sprintf(buffer, " %g\t", values[iR][iC]);
				s += buffer;
			}
			s += "\r\n";
		}
		return s;
	}

	CMatrix CMatrix ::operator = (const CMatrix & m) {
		copy(m);
		return *this;
	}
	CMatrix CMatrix ::operator = (double d) {
		copy(d);
		return *this;
	}
	CMatrix CMatrix ::operator = (string s) {
		copy(s);
		return *this;
	}

	void CMatrix ::add(const CMatrix & m) {
		if(nR!= m.nR || nC!= m.nC)
			throw ("Invalid matrix dimension");
		for (int iR = 0; iR < nR; iR++)
			for (int iC = 0; iC < nC; iC++)
				values[iR][iC] += m.values[iR][iC];
	}
	void CMatrix ::operator += (const CMatrix & m) {
		add(m);
	}
	void CMatrix ::operator += (double d) {
		add(CMatrix(nR, nC, MI_VALUE, d));
	}
	CMatrix CMatrix ::operator + (const CMatrix & m) {
		CMatrix r = *this;
		r += m;
		return r;
	}
	CMatrix CMatrix ::operator + (double d) {
		CMatrix r = *this;
		r += d;
		return r;
	}

	void CMatrix ::sub(const CMatrix & m) {
		if(nR!= m.nR || nC!= m.nC)
			throw ("Invalid matrix dimension");
		for (int iR = 0; iR < nR; iR++)
			for (int iC = 0; iC < nC; iC++)
				values[iR][iC] -= m.values[iR][iC];
	}
	void CMatrix ::operator -= (const CMatrix & m) {
		sub(m);
	}
	void CMatrix ::operator -= (double d) {
		sub(CMatrix(nR, nC, MI_VALUE, d));
	}
	CMatrix CMatrix ::operator - (const CMatrix & m) {
		CMatrix r = *this;
		r -= m;
		return r;
	}
	CMatrix CMatrix ::operator - (double d) {
		CMatrix r = *this;
		r -= d;
		return r;
	}

	void CMatrix ::mul(const CMatrix & m) {
		if(nR!= m.nR || nC!= m.nC)
			throw ("Invalid matrix dimension");
		CMatrix r(nR, m.nC);
		for (int iR = 0; iR < r.nR; iR++)
			for (int iC = 0; iC < r.nC; iC++) {
				r.values[iR][iC] = 0;
				for (int k = 0; k < m.nC; k++)
					r.values[iR][iC] += values[iR][k] * m.values[k][iC];
			}
		copy(r);
	}
	void CMatrix ::operator *= (const CMatrix & m) {
		mul(m);
	}
	void CMatrix ::operator *= (double d) {
		for (int iR = 0; iR < nR; iR++)
			for (int iC = 0; iC < nC; iC++)
				values[iR][iC] *= d;
	}
	CMatrix CMatrix ::operator * (const CMatrix & m) {
		CMatrix r = *this;
		r *= m;
		return r;
	}
	CMatrix CMatrix ::operator * (double d) {
		CMatrix r = *this;
		r *= d;
		return r;
	}

	CMatrix CMatrix ::operator++() {
		add(CMatrix(nR, nC, MI_VALUE, 1.0));
		return *this;
	}
	CMatrix CMatrix ::operator++(int) {
		CMatrix C = *this;
		add(CMatrix(nR, nC, MI_VALUE, 1.0));
		return C;
	}
	CMatrix CMatrix ::operator --() {
		add(CMatrix(nR, nC, MI_VALUE, -1.0));
		return *this;
	}
	CMatrix CMatrix ::operator --(int) {
		CMatrix r = *this;
		add(CMatrix(nR, nC, MI_VALUE, -1.0));
		return r;
	}
	CMatrix CMatrix ::operator - () {
		for (int iR = 0; iR < nR; iR++)
			for (int iC = 0; iC < nC; iC++)
				values[iR][iC] = -values[iR][iC];
		return *this;
	}
	CMatrix CMatrix ::operator + () {
		return *this;
	}

	void CMatrix ::setSubMatrix(int r, int c, const CMatrix & m) {
		if((r + m.nR) > nR || (c + m.nC) > nC) throw ("Invalid matrix dimension");
		for (int iR = 0; iR < m.nR; iR++)
			for (int iC = 0; iC < m.nC; iC++)
				values[r + iR][c + iC] = m.values[iR][iC];
	}
	CMatrix CMatrix ::getSubMatrix(int r, int c, int nr, int nc) {
		if((r + nr) > nR || (c + nc) > nC) throw ("Invalid matrix dimension");
		CMatrix m(nr, nc);
		for (int iR = 0; iR < m.nR; iR++)
			for (int iC = 0; iC < m.nC; iC++)
				m.values[iR][iC] = values[r + iR][c + iC];
		return m;
	}

	void CMatrix ::addColumn(const CMatrix & m) {
		CMatrix n(max(nR, m.nR), nC + m.nC);
		n.setSubMatrix(0, 0, *this);
		n.setSubMatrix(0, nC, m);
		*this = n;
	}
	void CMatrix ::addRow(const CMatrix & m) {
		CMatrix n(nR + m.nR, max(nC, m.nC));
		n.setSubMatrix(0, 0, *this);
		n.setSubMatrix(nR, 0, m); 
		*this = n;
	}

	CMatrix CMatrix ::getCofactor(int r, int c) {
		if(nR <= 1 && nC <= 1) throw ("Invalid matrix dimension");
		CMatrix m(nR - 1, nC - 1);
		for (int iR = 0; iR < m.nR; iR++)
			for (int iC = 0; iC < m.nC; iC++) {
				int sR = (iR < r) ? iR : iR + 1;
				int sC = (iC < c) ? iC : iC + 1;
				m.values[iR][iC] = values[sR][sC];
			}
		return m;
	}

	double CMatrix ::getDeterminant() {
		if(nR!= nC) throw ("Invalid matrix dimension");
		if(nR == 1 && nC == 1) return values[0][0];
		double value = 0, m = 1;
		for (int iR = 0; iR < nR; iR++) {
			value += m * values[0][iR] * getCofactor(0, iR).getDeterminant();
			m *= -1;
		}
		return value;
	}
	///////////////////////////////////////////////////////////////////////////////
	CMatrix CMatrix::getTranspose()
	{
		CMatrix a(nC, nR);
		for (int iR = 0; iR < a.nR; iR++)
			for (int iC = 0; iC < a.nC; iC++)
				a.values[iR][iC] = values[iC][iR];

		return a;
	}

	CMatrix CMatrix::getInverse()
	{
		if (nR != nC) throw ("Invalid matrix dimension");
		CMatrix m(nR, nC);
		for (int iR = 0; iR < m.nR; iR++)
			for (int iC = 0; iC < m.nC; iC++) 
			{
				if (iR % 2 == 0) {
					m.values[iR][iC] = (iC % 2 == 0) ? getCofactor(iR, iC).getDeterminant() : -getCofactor(iR, iC).getDeterminant();
				}
				else
					m.values[iR][iC] = (iC % 2 == 0) ? -getCofactor(iR, iC).getDeterminant() : getCofactor(iR, iC).getDeterminant();
			}
		m = m.getTranspose();
		m = m * (1 / getDeterminant());
		return m;
	}
	/////////////////////////////////////////////////////////////////////////////
	istream & operator >> (istream &is, CMatrix & m) {
		string s;
		getline(is, s, ']');
		s += "]";
		m = CMatrix(s);
		return is;
	}
	ostream & operator << (ostream & os, CMatrix & m) {
		os << m.getString();
		return os;
	}

bool getIndexAndCheckIfExist(vector<string> &matrixNames,
	vector<CMatrix> &matrix,
	string &firstParmeter, int &firstIndex, bool &found1,
	string &secondParmeter, int &secondIndex, bool &found2) {

	found1 = false;
	for (int i = 0; i < matrixNames.size(); i++) {
		if (firstParmeter == matrixNames[i]) {
			firstIndex = i;
			if (firstIndex >= matrix.size())
				found1 = false;
			else
				found1 = true;

			break;
		}
	}
	found2 = false;
	for (int i = 0; i < matrixNames.size(); i++) {
		if (secondParmeter == matrixNames[i]) {
			secondIndex = i;
			if (secondIndex >= matrix.size())
				found2 = false;
			else
				found2 = true;

			break;
		}
	}

	bool doBreak = false;

	if (!found1 && !found2) {
		cout << firstParmeter << " and "
			<< secondParmeter << " aren't Intiallized" << endl;
		doBreak = true;
	}
	if (!found1 && !doBreak) {
		cout << firstParmeter << " is not Intiallized" << endl;
		doBreak = true;
	}
	if (!found2 && !doBreak) {
		cout << secondParmeter << " is not Intiallized" << endl;
		doBreak = true;
	}

	return doBreak;
}

bool checkAlpha(const string &str) {
	for (int i = 0; i < str.size(); i++)
		if (!isalpha(str[i]) || !isspace(str[i]))
			return true;
	return false;
}

int main() {

	vector<string> matrixNames;
	vector<CMatrix> matrix;
	vector<long> existingIndex;
	vector<bool> existingFlag;

	string line = "";
	int numlines = 0;

	while (1) {
		getline(cin, line);
		if (line == "end" || line == "end;") {
			cout << "thanks, good bye..." << endl;
			break;
		}
		if (line.empty()) {
			continue;
		}
		if (line.find(";") == string::npos) {
			cout << "missing ending semi colone" << endl;
			continue;
		}

		line = line.substr(0, line.length() - 1);
		size_t n = std::count(line.begin(), line.end(), '=');
		int x = 0;
		if (n == 0) {
			if (matrixNames.size() == 0) {
				cout << "unintiallized matrix" << endl;
			}
			for (int i = 0; i < matrixNames.size(); i++) {
				if (matrixNames[i] == line && i < matrix.size()) {
					cout << matrix[i];
					break;
				}
				if (i == matrixNames.size() - 1) {
					cout << "unintiallized matrix" << endl;
				}
			}
		}

		for (int i = 0; i < n; i++) {
			string l = (line.substr(0, line.find('=')));
			string::iterator end_pos = remove(l.begin(), l.end(), ' ');
			l.erase(end_pos, l.end());
			if (!matrixNames.size()) {
				existingFlag.push_back(false);
			}
			for (int i = 0; i < matrixNames.size(); i++) {
				if (l == matrixNames[i]) {
					existingFlag.push_back(true);
					existingIndex.push_back(i);
					break;
				}
				if (i == matrixNames.size() - 1) {
					existingFlag.push_back(false);
				}
			}

			if (!existingFlag[numlines + i]) {
				matrixNames.push_back(l);
				existingIndex.push_back(i);
			}
			else {
				matrixNames.push_back(l);
			}
			line = line.substr(line.find('=') + 1);
		}

		for (int i = 0; i < n; i++) {
			int a = line.find("[");
			if (line.find('[') != string::npos) {
				if (!existingFlag[numlines + i]) {
					matrix.push_back(line);
				}
				else {
					matrix.push_back(NULL);
					matrix[existingIndex[numlines + i]] = line;
				}

			}
			else if (line.find('[') == string::npos) {

				//removing spaces
				string::iterator end_pos = remove(line.begin(), line.end(), ' ');
				line.erase(end_pos, line.end());

				if (line.find('+') != string::npos) {
					string firstparmeter = line.substr(0, line.find('+'));
					string secondparmeter = line.substr(line.find('+') + 1);
					int firstindex, secondindex;
					bool found1, found2;

					if (getIndexAndCheckIfExist(matrixNames, matrix,
						firstparmeter, firstindex, found1,
						secondparmeter, secondindex, found2))
						break;

					if (!existingFlag[numlines + i]) {
						matrix.push_back(matrix[firstindex] + matrix[secondindex]);
					}
					else {
						matrix.push_back(NULL);
						matrix[existingIndex[numlines + i]] = matrix[firstindex] + matrix[secondindex];
					}
				}
				else if (line.find('-') != string::npos) {
					string firstparmeter = line.substr(0, line.find('-'));
					string secondparmeter = line.substr(line.find('-') + 1);
					int firstindex, secondindex;
					bool found1, found2;

					if (getIndexAndCheckIfExist(matrixNames, matrix,
						firstparmeter, firstindex, found1,
						secondparmeter, secondindex, found2))
						break;

					if (!existingFlag[numlines + i]) {
						matrix.push_back(matrix[firstindex] - matrix[secondindex]);
					}
					else {
						matrix.push_back(NULL);
						matrix[existingIndex[numlines + i]] = matrix[firstindex] - matrix[secondindex];
					}

				}
				else if (line.find('*') != string::npos) {
					string firstparmeter = line.substr(0, line.find('*'));
					string secondparmeter = line.substr(line.find('*') + 1);
					int firstindex, secondindex;
					bool found1, found2;

					if (getIndexAndCheckIfExist(matrixNames, matrix,
						firstparmeter, firstindex, found1,
						secondparmeter, secondindex, found2))
						break;

					if (!existingFlag[numlines + i]) {
						matrix.push_back(matrix[firstindex] * matrix[secondindex]);
					}
					else {
						matrix.push_back(NULL);
						matrix[existingIndex[numlines + i]] = matrix[firstindex] * matrix[secondindex];
					}
				}
				else if (line.find('/') != string::npos) {
					cout << "operation not supported yet" << endl;
				}
				else if (line.find("'") != string::npos) {
					cout << "operation not supported yet" << endl;
				}
				else if (checkAlpha(line)) {

					bool found = false; int index;
					for (int i = 0; i < matrixNames.size(); i++) {
						if (line == matrixNames[i]) {
							index = i;
							if (index >= matrix.size())
								found = false;
							else
								found = true;
							break;
						}
					}

					if (!found) {
						cout << line << " is not Intiallized" << endl;
						break;
					}

					if (!existingFlag[numlines + i]) {
						matrix.push_back(matrix[index] + 0);
					}
					else {
						matrix.push_back(NULL);
						matrix[existingIndex[numlines + i]] = matrix[index];
					}

				}
				else {
					cout << "operation not found" << endl;
				}
			}
		}

		numlines += n;
	}





	return 0;
}

//
//bool found1 = false;
//for (int i = 0; i < matrixNames.size(); i++) {
//	if (firstParmeter == matrixNames[i]) {
//		firstIndex = i;
//		if (firstIndex >= matrix.size())
//			found1 = false;
//		else
//			found1 = true;
//
//		break;
//	}
//}
//bool found2 = false;
//for (int i = 0; i < matrixNames.size(); i++) {
//	if (secondParmeter == matrixNames[i]) {
//		secondIndex = i;
//		if (secondIndex >= matrix.size())
//			found2 = false;
//		else
//			found2 = true;
//
//		break;
//	}
//}
//if (!found1 && !found2) {
//	cout << firstParmeter << " and "
//		<< secondParmeter << " aren't Intiallized" << endl;
//	break;
//}
//if (!found1) {
//	cout << firstParmeter << " is not Intiallized" << endl;
//	break;
//}
//if (!found2) {
//	cout << secondParmeter << " is not Intiallized" << endl;
//	break;
//}
//
//if (!existingFlag[NumLines + i]) {
//	matrix.push_back(matrix[firstIndex] + matrix[secondIndex]);
//}
//else {
//	matrix.push_back(NULL);
//	matrix[existingIndex[NumLines + i]] = matrix[firstIndex] + matrix[secondIndex];
//}


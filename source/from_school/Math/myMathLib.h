#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <string>
#include <vector>

class Vector4D
{
private:
	float vectorArr[4];
public:
	inline Vector4D();
	inline Vector4D(float x, float y, float z); /**Initialisera en Vector4D med v�rdena x, y, z, 1 */
	inline Vector4D(float x, float y, float z, float w); /**Initialisera en Vector4D med v�rdena x, y, z, w */
	inline Vector4D operator+(const Vector4D& addWith); /**Returnerar en ny Vector4D med v�rdena x = operand1.x + operand2.x, y = operand1.y + operand2.y, z = operand1.z + operand2.z, 1 */
	inline Vector4D operator-(const Vector4D& subtractWith); /**Returnerar en ny Vector4D med v�rdena x = operand1.x - operand2.x, y = operand1.y - operand2.y, z = operand1.z - operand2.z, 1 */
	inline Vector4D operator*(const float& multiplyWith); /**Returnerar en ny Vector4D d�r operand1 har blivit f�rl�ngd med operand2 i.e. varje v�rde hos operand1 har blivit multiplicerad med operand2 f�rutom w */
	inline float operator*(const Vector4D& productWith); /**Returnerar skal�rprodukten av operand1 och operand2 */
	inline Vector4D CrossProduct(const Vector4D& productWith); /**Returnerar kryssprodukten av operand1 och operand2 */
	inline float operator[](int i)const; /**L�ter dig h�mta ut v�rdet p� plats 'i' inuti arrayen inom klassen */
	inline float GetLength(); /**Returner l�ngden av Vektor4D */
	inline void Normalize(); /**G�r l�ngden av vecktorn som kallar funktionen till 1 l�ngdenhet */
	inline float* GetArray();
	inline std::vector<float> GetVector();
	inline void DivideByW();
};
class Matrix4D
{
private:
	float matrixArr[4 * 4];
public:
	inline Matrix4D(); /**Initialiserar en identitetsmatris */
	inline Matrix4D(float arr3x3[9]); /**Initialiserar en 4x4 identitietsmatris med de tre f�rsta raderna av de tre f�rsta columnerna utbytta mot medf�ljande 3x3 matris */
	inline Matrix4D(std::vector<float> arr4x4); /**Initialiserar en 4x4 identitietsmatris med medf�ljande 4x4 matris array */
	inline static Matrix4D CreateMatrixRotX(float radians); /**Returnerar en matris som roterar runt x-axeln med de antal radianer specifierade */
	inline static Matrix4D CreateMatrixRotY(float radians); /**Returnerar en matris som roterar runt y-axeln med de antal radianer specifierade */
	inline static Matrix4D CreateMatrixRotZ(float radians); /**Returnerar en matris som roterar runt z-axeln med de antal radianer specifierade */
	inline static Matrix4D CreateMatrixRotVector(float radians, float x, float y, float z); /**Returnerar en matris som roterar runt en vector(x, y, z) med de antal radianer specifierade */
	inline Matrix4D operator*(const Matrix4D& multiplyWith); /**Returnerar en ny matris som �r operand1 multiplicerad med operand2*/
	inline Vector4D operator*(const Vector4D& multiplyWith); /**Returnerar en Vector4D som �r operand1(matris) multiplicerad med operand2(vektor) */
	inline static Matrix4D Transpose(Matrix4D transposeTarget); /**Returnerar en ny matris som �r den inskickade matrisen transponerad i.e. raderna och kolumnerna har bytt plats */
	inline static Matrix4D InverseMatrix(const Matrix4D& inverseTarget); /**Returnerar en ny matris som �r inversen till den inskickade matrisen */
	inline float operator[](int i)const; /**L�ter dig h�mta ut v�rdet p� plats 'i' inuti arrayen inom klassen */
	inline float* GetArray();
	inline static Matrix4D SetTranslation(float x, float y, float z);
	inline static Matrix4D CreateScalingMatrix(float scaleX, float scaleY, float scaleZ);
	inline static Matrix4D CreateMatrixRotFromQuaternion(float x, float y, float z, float w);
};
Matrix4D::Matrix4D()
{
	matrixArr[0] = 1;
	matrixArr[1] = 0;
	matrixArr[2] = 0;
	matrixArr[3] = 0;
	matrixArr[4] = 0;
	matrixArr[5] = 1;
	matrixArr[6] = 0;
	matrixArr[7] = 0;
	matrixArr[8] = 0;
	matrixArr[9] = 0;
	matrixArr[10] = 1;
	matrixArr[11] = 0;
	matrixArr[12] = 0;
	matrixArr[13] = 0;
	matrixArr[14] = 0;
	matrixArr[15] = 1;
}
Matrix4D::Matrix4D(float arr3x3[9])
{
	matrixArr[0] = arr3x3[0];
	matrixArr[1] = arr3x3[1];
	matrixArr[2] = arr3x3[2];
	matrixArr[3] = 0;
	matrixArr[4] = arr3x3[3];
	matrixArr[5] = arr3x3[4];
	matrixArr[6] = arr3x3[5];
	matrixArr[7] = 0;
	matrixArr[8] = arr3x3[6];
	matrixArr[9] = arr3x3[7];
	matrixArr[10] = arr3x3[8];
	matrixArr[11] = 0;
	matrixArr[12] = 0;
	matrixArr[13] = 0;
	matrixArr[14] = 0;
	matrixArr[15] = 1;
}
Matrix4D::Matrix4D(std::vector<float> arr4x4)
{
	matrixArr[0] = arr4x4[0];
	matrixArr[1] = arr4x4[1];
	matrixArr[2] = arr4x4[2];
	matrixArr[3] = arr4x4[3];
	matrixArr[4] = arr4x4[4];
	matrixArr[5] = arr4x4[5];
	matrixArr[6] = arr4x4[6];
	matrixArr[7] = arr4x4[7];
	matrixArr[8] = arr4x4[8];
	matrixArr[9] = arr4x4[9];
	matrixArr[10] = arr4x4[10];
	matrixArr[11] = arr4x4[11];
	matrixArr[12] = arr4x4[12];
	matrixArr[13] = arr4x4[13];
	matrixArr[14] = arr4x4[14];
	matrixArr[15] = arr4x4[15];
}
Matrix4D Matrix4D::CreateMatrixRotX(float radians)
{
	float mCos = cosf(radians);
	float mSin = sinf(radians);

	Matrix4D newMatrix;
	newMatrix.matrixArr[5] = mCos;
	newMatrix.matrixArr[6] = -mSin;
	newMatrix.matrixArr[9] = mSin;
	newMatrix.matrixArr[10] = mCos;
	return newMatrix;
}
Matrix4D Matrix4D::CreateMatrixRotY(float radians)
{
	float mCos = cosf(radians);
	float mSin = sinf(radians);

	Matrix4D newMatrix;
	newMatrix.matrixArr[0] = mCos;
	newMatrix.matrixArr[2] = mSin;
	newMatrix.matrixArr[8] = -mSin;
	newMatrix.matrixArr[10] = mCos;
	return newMatrix;
}
Matrix4D Matrix4D::CreateMatrixRotZ(float radians)
{
	float mCos = cosf(radians);
	float mSin = sinf(radians);

	Matrix4D newMatrix;
	newMatrix.matrixArr[0] = mCos;
	newMatrix.matrixArr[1] = -mSin;
	newMatrix.matrixArr[4] = mSin;
	newMatrix.matrixArr[5] = mCos;
	return newMatrix;
}
Matrix4D Matrix4D::CreateMatrixRotVector(float radians, float x, float y, float z)
{
	float mCos = cosf(radians);
	float mSin = sinf(radians);

	Matrix4D newMatrix;
	newMatrix.matrixArr[0] = x * x + (1 - x * x) * mCos;
	newMatrix.matrixArr[1] = x * y * (1 - mCos) - x * mSin;
	newMatrix.matrixArr[2] = x * z * (1 - mCos) + y * mSin;
	newMatrix.matrixArr[4] = x * y * (1 - mCos) + z * mSin;
	newMatrix.matrixArr[5] = y * y + (1 - y * y) * mCos;
	newMatrix.matrixArr[6] = x * z * (1 - mCos) - x * mSin;
	newMatrix.matrixArr[8] = x * z * (1 - mCos) - y * mSin;
	newMatrix.matrixArr[9] = y * z * (1 - mCos) + x * mSin;
	newMatrix.matrixArr[10] = z * z + (1 - z * z) * mCos;
	return newMatrix;
}
float Matrix4D::operator[](int i)const
{
	return matrixArr[i];
}
Matrix4D Matrix4D::operator*(const Matrix4D& multiplyWith)
{
	Matrix4D newMatrix;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			newMatrix.matrixArr[i * 4 + j] = matrixArr[i * 4] * multiplyWith[j] + matrixArr[i * 4 + 1] * multiplyWith[j + 4] + matrixArr[i * 4 + 2] * multiplyWith[j + 8] + matrixArr[i * 4 + 3] * multiplyWith[j + 12];
		}
	}
	return newMatrix;
}
Vector4D Matrix4D::operator*(const Vector4D& multiplyWith)
{
	float newVector[4];
	for (int i = 0; i < 4; i++)
	{
		newVector[i] = matrixArr[i * 4] * multiplyWith[0] + matrixArr[i * 4 + 1] * multiplyWith[1] + matrixArr[i * 4 + 2] * multiplyWith[2] + matrixArr[i * 4 + 3] * multiplyWith[3];
	}
	return Vector4D(newVector[0], newVector[1], newVector[2], newVector[3]);
}
Matrix4D Matrix4D::Transpose(Matrix4D transposeTarget)
{
	float temp = transposeTarget.matrixArr[1];
	transposeTarget.matrixArr[1] = transposeTarget.matrixArr[4];
	transposeTarget.matrixArr[4] = temp;

	temp = transposeTarget.matrixArr[2];
	transposeTarget.matrixArr[2] = transposeTarget.matrixArr[8];
	transposeTarget.matrixArr[8] = temp;

	temp = transposeTarget.matrixArr[3];
	transposeTarget.matrixArr[3] = transposeTarget.matrixArr[12];
	transposeTarget.matrixArr[12] = temp;

	temp = transposeTarget.matrixArr[6];
	transposeTarget.matrixArr[6] = transposeTarget.matrixArr[9];
	transposeTarget.matrixArr[9] = temp;

	temp = transposeTarget.matrixArr[7];
	transposeTarget.matrixArr[7] = transposeTarget.matrixArr[13];
	transposeTarget.matrixArr[13] = temp;

	temp = transposeTarget.matrixArr[11];
	transposeTarget.matrixArr[11] = transposeTarget.matrixArr[14];
	transposeTarget.matrixArr[14] = temp;

	return transposeTarget;
}
Matrix4D Matrix4D::InverseMatrix(const Matrix4D& inverseTarget)
{
	Matrix4D matrixOfMinors;
	matrixOfMinors.matrixArr[0]		=	inverseTarget[5] * (inverseTarget[10] * inverseTarget[15] - inverseTarget[11] * inverseTarget[14]) - inverseTarget[6] * (inverseTarget[9] * inverseTarget[15] - inverseTarget[11] * inverseTarget[13]) + inverseTarget[7] * (inverseTarget[9] * inverseTarget[14] - inverseTarget[10] * inverseTarget[13]);
	matrixOfMinors.matrixArr[1]		= -(inverseTarget[4] * (inverseTarget[10] * inverseTarget[15] - inverseTarget[11] * inverseTarget[14]) - inverseTarget[6] * (inverseTarget[8] * inverseTarget[15] - inverseTarget[11] * inverseTarget[12]) + inverseTarget[7] * (inverseTarget[8] * inverseTarget[14] - inverseTarget[10] * inverseTarget[12]));
	matrixOfMinors.matrixArr[2]		=	inverseTarget[4] * (inverseTarget[9] * inverseTarget[15] - inverseTarget[11] * inverseTarget[13]) - inverseTarget[5] * (inverseTarget[8] * inverseTarget[15] - inverseTarget[11] * inverseTarget[12]) + inverseTarget[7] * (inverseTarget[8] * inverseTarget[13] - inverseTarget[9] * inverseTarget[12]);
	matrixOfMinors.matrixArr[3]		= -(inverseTarget[4] * (inverseTarget[9] * inverseTarget[14] - inverseTarget[10] * inverseTarget[13]) - inverseTarget[5] * (inverseTarget[8] * inverseTarget[14] - inverseTarget[10] * inverseTarget[12]) + inverseTarget[6] * (inverseTarget[8] * inverseTarget[13] - inverseTarget[9] * inverseTarget[12]));
	matrixOfMinors.matrixArr[4]		= -(inverseTarget[1] * (inverseTarget[10] * inverseTarget[15] - inverseTarget[11] * inverseTarget[14]) - inverseTarget[2] * (inverseTarget[9] * inverseTarget[15] - inverseTarget[11] * inverseTarget[13]) + inverseTarget[3] * (inverseTarget[9] * inverseTarget[14] - inverseTarget[10] * inverseTarget[13]));
	matrixOfMinors.matrixArr[5]		=	inverseTarget[0] * (inverseTarget[10] * inverseTarget[15] - inverseTarget[11] * inverseTarget[14]) - inverseTarget[2] * (inverseTarget[8] * inverseTarget[15] - inverseTarget[11] * inverseTarget[12]) + inverseTarget[3] * (inverseTarget[8] * inverseTarget[14] - inverseTarget[10] * inverseTarget[12]);
	matrixOfMinors.matrixArr[6]		= -(inverseTarget[0] * (inverseTarget[9] * inverseTarget[15] - inverseTarget[11] * inverseTarget[13]) - inverseTarget[1] * (inverseTarget[8] * inverseTarget[15] - inverseTarget[11] * inverseTarget[12]) + inverseTarget[3] * (inverseTarget[8] * inverseTarget[13] - inverseTarget[9] * inverseTarget[12]));
	matrixOfMinors.matrixArr[7]		=	inverseTarget[0] * (inverseTarget[9] * inverseTarget[14] - inverseTarget[10] * inverseTarget[13]) - inverseTarget[1] * (inverseTarget[8] * inverseTarget[14] - inverseTarget[10] * inverseTarget[12]) + inverseTarget[2] * (inverseTarget[8] * inverseTarget[13] - inverseTarget[9] * inverseTarget[12]);
	matrixOfMinors.matrixArr[8]		=	inverseTarget[1] * (inverseTarget[6] * inverseTarget[15] - inverseTarget[7] * inverseTarget[14]) - inverseTarget[2] * (inverseTarget[5] * inverseTarget[15] - inverseTarget[7] * inverseTarget[13]) + inverseTarget[3] * (inverseTarget[5] * inverseTarget[14] - inverseTarget[6] * inverseTarget[13]);
	matrixOfMinors.matrixArr[9]		= -(inverseTarget[0] * (inverseTarget[6] * inverseTarget[15] - inverseTarget[7] * inverseTarget[14]) - inverseTarget[2] * (inverseTarget[4] * inverseTarget[15] - inverseTarget[7] * inverseTarget[12]) + inverseTarget[3] * (inverseTarget[4] * inverseTarget[14] - inverseTarget[6] * inverseTarget[12]));
	matrixOfMinors.matrixArr[10]	=	inverseTarget[0] * (inverseTarget[5] * inverseTarget[15] - inverseTarget[7] * inverseTarget[13]) - inverseTarget[1] * (inverseTarget[4] * inverseTarget[15] - inverseTarget[7] * inverseTarget[12]) + inverseTarget[3] * (inverseTarget[4] * inverseTarget[13] - inverseTarget[5] * inverseTarget[12]);
	matrixOfMinors.matrixArr[11]	= -(inverseTarget[0] * (inverseTarget[5] * inverseTarget[14] - inverseTarget[6] * inverseTarget[13]) - inverseTarget[1] * (inverseTarget[4] * inverseTarget[14] - inverseTarget[6] * inverseTarget[12]) + inverseTarget[2] * (inverseTarget[4] * inverseTarget[13] - inverseTarget[5] * inverseTarget[12]));
	matrixOfMinors.matrixArr[12]	= -(inverseTarget[1] * (inverseTarget[6] * inverseTarget[11] - inverseTarget[7] * inverseTarget[10]) - inverseTarget[2] * (inverseTarget[5] * inverseTarget[11] - inverseTarget[7] * inverseTarget[9]) + inverseTarget[3] * (inverseTarget[5] * inverseTarget[10] - inverseTarget[6] * inverseTarget[9]));
	matrixOfMinors.matrixArr[13]	=	inverseTarget[0] * (inverseTarget[6] * inverseTarget[11] - inverseTarget[7] * inverseTarget[10]) - inverseTarget[2] * (inverseTarget[4] * inverseTarget[11] - inverseTarget[7] * inverseTarget[8]) + inverseTarget[3] * (inverseTarget[4] * inverseTarget[10] - inverseTarget[6] * inverseTarget[8]);
	matrixOfMinors.matrixArr[14]	= -(inverseTarget[0] * (inverseTarget[5] * inverseTarget[11] - inverseTarget[7] * inverseTarget[9]) - inverseTarget[1] * (inverseTarget[4] * inverseTarget[11] - inverseTarget[7] * inverseTarget[8]) + inverseTarget[3] * (inverseTarget[4] * inverseTarget[9] - inverseTarget[5] * inverseTarget[8]));
	matrixOfMinors.matrixArr[15]	=	inverseTarget[0] * (inverseTarget[5] * inverseTarget[10] - inverseTarget[6] * inverseTarget[9]) - inverseTarget[1] * (inverseTarget[4] * inverseTarget[10] - inverseTarget[6] * inverseTarget[8]) + inverseTarget[2] * (inverseTarget[4] * inverseTarget[9] - inverseTarget[5] * inverseTarget[8]);

	float determinantDivided = 1 / (inverseTarget[0] * (inverseTarget[5] * (inverseTarget[10] * inverseTarget[15] - inverseTarget[11] * inverseTarget[14]) - inverseTarget[6] * (inverseTarget[9] * inverseTarget[15] - inverseTarget[11] * inverseTarget[13]) + inverseTarget[7] * (inverseTarget[9] * inverseTarget[14] - inverseTarget[10] * inverseTarget[13])) - inverseTarget[1] * (inverseTarget[4] * (inverseTarget[10] * inverseTarget[15] - inverseTarget[11] * inverseTarget[14]) - inverseTarget[6] * (inverseTarget[8] * inverseTarget[15] - inverseTarget[11] * inverseTarget[12]) + inverseTarget[7] * (inverseTarget[8] * inverseTarget[14] - inverseTarget[10] * inverseTarget[12])) + inverseTarget[2] * (inverseTarget[4] * (inverseTarget[9] * inverseTarget[15] - inverseTarget[11] * inverseTarget[13]) - inverseTarget[5] * (inverseTarget[8] * inverseTarget[15] - inverseTarget[11] * inverseTarget[12]) + inverseTarget[7] * (inverseTarget[8] * inverseTarget[13] - inverseTarget[9] * inverseTarget[12])) - inverseTarget[3] * (inverseTarget[4] * (inverseTarget[9] * inverseTarget[14] - inverseTarget[10] * inverseTarget[13]) - inverseTarget[5] * (inverseTarget[8] * inverseTarget[14] - inverseTarget[10] * inverseTarget[12]) + inverseTarget[6] * (inverseTarget[8] * inverseTarget[13] - inverseTarget[9] * inverseTarget[12])));
	matrixOfMinors = Matrix4D::Transpose(matrixOfMinors);
	for (int i = 0; i < 16; i++)
	{
		matrixOfMinors.matrixArr[i] = determinantDivided * matrixOfMinors.matrixArr[i];
	}
	return matrixOfMinors;
}

float* Matrix4D::GetArray()
{
	return matrixArr;
}

Matrix4D Matrix4D::SetTranslation(float x, float y, float z)
{
	Matrix4D newMatrix;
	newMatrix.matrixArr[3] = x;
	newMatrix.matrixArr[7] = y;
	newMatrix.matrixArr[11] = z;
	return newMatrix;
}

Matrix4D Matrix4D::CreateScalingMatrix(float scaleX, float scaleY, float scaleZ)
{
	Matrix4D newMatrix;
	newMatrix.matrixArr[0] = scaleX;
	newMatrix.matrixArr[5] = scaleY;
	newMatrix.matrixArr[10] = scaleZ;
	return newMatrix;
}

Matrix4D Matrix4D::CreateMatrixRotFromQuaternion(float x, float y, float z, float w) {
	Matrix4D mat;
	mat.matrixArr[0] = 1 - 2 * y * y - 2 * z * z;
	mat.matrixArr[1] = 2 * x * y + 2 * w * z;
	mat.matrixArr[2] = 2 * x * z - 2 * w * y;
	mat.matrixArr[3] = 0;
	mat.matrixArr[4] = 2 * x * y - 2 * w * z;
	mat.matrixArr[5] = 1 - 2 * x * x - 2 * z * z;
	mat.matrixArr[6] = 2 * y * z + 2 * w * x;
	mat.matrixArr[7] = 0;
	mat.matrixArr[8] = 2 * x * z + 2 * w * y;
	mat.matrixArr[9] = 2 * y * z - 2 * w * x;
	mat.matrixArr[10] = 1 - 2 * x * x - 2 * y * y;
	mat.matrixArr[11] = 0;
	mat.matrixArr[12] = 0;
	mat.matrixArr[13] = 0;
	mat.matrixArr[14] = 0;
	mat.matrixArr[15] = 1;
	return mat;
}

Vector4D::Vector4D()
{
	vectorArr[0] = 0;
	vectorArr[1] = 0;
	vectorArr[2] = 0;
	vectorArr[3] = 1;
}

Vector4D::Vector4D(float x, float y, float z)
{
	vectorArr[0] = x;
	vectorArr[1] = y;
	vectorArr[2] = z;
	vectorArr[3] = 1;
}
Vector4D::Vector4D(float x, float y, float z, float w)
{
	vectorArr[0] = x;
	vectorArr[1] = y;
	vectorArr[2] = z;
	vectorArr[3] = w;
}
float Vector4D::operator[](int i)const
{
	return vectorArr[i];
}
Vector4D Vector4D::operator+(const Vector4D& addWith)
{
	return Vector4D(vectorArr[0] + addWith[0], vectorArr[1] + addWith[1], vectorArr[2] + addWith[2]);
}
Vector4D Vector4D::operator-(const Vector4D& subtractWith)
{
	return Vector4D(vectorArr[0] - subtractWith[0], vectorArr[1] - subtractWith[1], vectorArr[2] - subtractWith[2]);
}
Vector4D Vector4D::operator*(const float& multiplyWith)
{
	return Vector4D(vectorArr[0] * multiplyWith, vectorArr[1] * multiplyWith, vectorArr[2] * multiplyWith);
}
float Vector4D::operator*(const Vector4D& productWith)
{
	return vectorArr[0] * productWith[0] + vectorArr[1] * productWith[1] + vectorArr[2] * productWith[2];
}
Vector4D Vector4D::CrossProduct(const Vector4D& productWith)
{
	return Vector4D(vectorArr[1] * productWith[2] - vectorArr[2] * productWith[1], vectorArr[2] * productWith[0] - vectorArr[0] * productWith[2], vectorArr[0] * productWith[1] - vectorArr[1] * productWith[0]);
}
float Vector4D::GetLength()
{
	return sqrtf(vectorArr[0] * vectorArr[0] + vectorArr[1] * vectorArr[1] + vectorArr[2] * vectorArr[2]);
}
void Vector4D::Normalize()
{
	float magnitude = 1.0f / this->GetLength();
	(*this) = (*this) * magnitude;
}

float* Vector4D::GetArray()
{
	return vectorArr;
}

std::vector<float> Vector4D::GetVector()
{
	std::vector<float> newV = std::vector<float>();
	newV.push_back(vectorArr[0]);
	newV.push_back(vectorArr[1]);
	newV.push_back(vectorArr[2]);
	newV.push_back(vectorArr[3]);
	return newV;
}

void Vector4D::DivideByW()
{
	vectorArr[0] = vectorArr[0] / vectorArr[3];
	vectorArr[1] = vectorArr[1] / vectorArr[3];
	vectorArr[2] = vectorArr[2] / vectorArr[3];
	vectorArr[3] = vectorArr[3] / vectorArr[3];
}
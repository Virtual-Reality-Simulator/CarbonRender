#include "..\Inc\CRMath.h"

float3::float3() {}

float3::float3(float a)
{
	x = a;
	y = a;
	z = a;
}

float3::float3(float a, float b, float c)
{
	x = a;
	y = b;
	z = c;
}

float3::float3(FbxDouble3 a)
{
	x = (float)a[0];
	y = (float)a[1];
	z = (float)a[2];
}

float3::float3(FbxDouble4 a)
{
	x = (float)a[0];
	y = (float)a[1];
	z = (float)a[2];
}

float3::float3(float4 a)
{
	x = a.x;
	y = a.y;
	z = a.z;
}

float3 float3::normalize()
{
	float l = sqrt(x*x + y*y + z*z);

	return float3(x / l,y / l,z / l);
}

void float3::operator=(FbxDouble3 a)
{
	x = (float)a[0];
	y = (float)a[1];
	z = (float)a[2];
}

void float3::operator=(FbxDouble4 a)
{
	x = (float)a[0];
	y = (float)a[1];
	z = (float)a[2];
}

void float3::operator=(float3 a)
{
	x = a.x;
	y = a.y;
	z = a.z;
}

void float3::operator=(float4 a)
{
	x = a.x;
	y = a.y;
	z = a.z;
}

float4::float4() {}

float4::float4(float a, float b, float c, float d)
{
	x = a;
	y = b;
	z = c;
	w = d;
}

float4::float4(float3 a, float d)
{
	x = a.x;
	y = a.y;
	z = a.z;
	w = d;
}

float4 float4::normalize()
{
	float l = sqrt(x*x + y*y + z*z + w*w);

	return float4(x / l, y / l, z / l, w / l);
}

void float4::operator=(float4 a)
{
	x = a.x;
	y = a.y;
	z = a.z;
	w = a.w;
}

Matrix3x3::Matrix3x3() {}

Matrix3x3::Matrix3x3(float a[9])
{
	for (int i = 0; i < 9; i++)
		matrix[i] = a[i];
}

Matrix3x3::Matrix3x3(Matrix4x4 &mat)
{
	for (int i = 0; i < 3; i++)
	{
		matrix[i * 3] = mat.matrix[i * 4];
		matrix[i * 3 + 1] = mat.matrix[i * 4 + 1];
		matrix[i * 3 + 2] = mat.matrix[i * 4 + 2];
	}
}

void Matrix3x3::operator=(float a[9])
{
	for (int i = 0; i < 9; i++)
		matrix[i] = a[i];
}

void Matrix3x3::operator=(Matrix3x3 mat)
{
	for (int i = 0; i < 9; i++)
		matrix[i] = mat.matrix[i];
}

void Matrix3x3::operator=(Matrix4x4 mat)
{
	for (int i = 0; i < 3; i++)
	{
		matrix[i * 3] = mat.matrix[i * 4];
		matrix[i * 3 + 1] = mat.matrix[i * 4 + 1];
		matrix[i * 3 + 2] = mat.matrix[i * 4 + 2];
	}
}

Matrix4x4::Matrix4x4(){}

Matrix4x4::Matrix4x4(float a[16])
{
	for (int i = 0; i < 16; i++)
		matrix[i] = a[i];
}

void Matrix4x4::operator=(float a[16])
{
	for (int i = 0; i < 16; i++)
		matrix[i] = a[i];
}

void Matrix4x4::operator=(Matrix4x4 mat)
{
	for (int i = 0; i < 16; i++)
		matrix[i] = mat.matrix[i];
}

Quaternion::Quaternion() {}

Quaternion::Quaternion(float a, float b, float c, float d)
{
	x = a;
	y = b;
	z = c;
	w = d;
}

Quaternion Quaternion::Normailze()
{
	float lengthR = 1.0f / sqrtf(x*x + y*y + z*z + w*w);
	return Quaternion(x*lengthR, y*lengthR, z*lengthR, w*lengthR);
}

Matrix4x4 Quaternion::ToMatrix()
{
	float r[16] = { 1.0f - 2 * (y*y + z*z), 2 * (x*y - z*w), 2 * (x*z + y*w), 0.0f,
					2 * (x*y + z*w), 1.0f -  2 * (x*x + z*z), 2 * (y*z - x*w), 0.0f,
					2 * (x*z - y*w), 2 * (y*z + x*w), 1.0f - 2 * (x*x + y*y), 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f };
	return Matrix4x4(r);
}

Matrix4x4 operator*(Matrix4x4 m1, Matrix4x4 m2)
{
	Matrix4x4 resultM;
	for (int i = 0; i < 16; i++)
	{
		float temp = (float)i / 4.0f;
		int r = (int)temp * 4;
		int l = (int)i % 4;
		resultM.matrix[i] = m1.matrix[0 + r] * m2.matrix[0 + l] +
							m1.matrix[1 + r] * m2.matrix[4 + l] +
							m1.matrix[2 + r] * m2.matrix[8 + l] +
							m1.matrix[3 + r] * m2.matrix[12 + l];
	}

	return resultM;
}

float4 operator*(float4 v, Matrix4x4 m)
{
	float4 result;
	result.x = v.x*m.matrix[0] + v.y*m.matrix[4] + v.z*m.matrix[8] + v.w*m.matrix[12];
	result.y = v.x*m.matrix[1] + v.y*m.matrix[5] + v.z*m.matrix[9] + v.w*m.matrix[13];
	result.z = v.x*m.matrix[2] + v.y*m.matrix[6] + v.z*m.matrix[10] + v.w*m.matrix[14];
	result.w = v.x*m.matrix[3] + v.y*m.matrix[7] + v.z*m.matrix[11] + v.w*m.matrix[15];

	return result;
}

float4 operator*(float4 v, Quaternion r)
{
	return float4(v.x*r.x, v.y*r.y, v.z*r.z, v.w*r.w);
}

float3 operator*(float3 v, float s)
{
	return float3(v.x*s, v.y*s, v.z*s);
}

float4 operator*(float4 v, float s)
{
	return float4(v.x*s, v.y*s, v.z*s, v.w*s);
}

float Dot(float3 a, float3 b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

float Dot(float4 a, float4 b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

Matrix4x4 Translate(float x, float y, float z)
{
	float tArray[16] = {1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						x, y, z, 1.0f };
	Matrix4x4 t(tArray);

	return t;
}

Matrix4x4 Scale(float x, float y, float z)
{
	float sArray[16] = {x, 0.0f, 0.0f, 0.0f,
						0.0f, y, 0.0f, 0.0f,
						0.0f, 0.0f, z, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f };
	Matrix4x4 s(sArray);

	return s;
}

Quaternion Rotate(float3 axis, float angle)
{
	float r = angle * 0.5f * A2R;
	return Quaternion(axis.x * sin(r),
					  axis.y * sin(r),
					  axis.z * sin(r),
					  cos(r));
}

Matrix4x4 CalculateModelMatrix(float3 trans, float3 rota, float3 scal)
{
	return  Scale(scal.x, scal.y, scal.z) *
			Rotate(float3(1.0f, 0.0f, 0.0f), rota.x).Normailze().ToMatrix() *
			Rotate(float3(0.0f, 1.0f, 0.0f), rota.y).Normailze().ToMatrix() *
			Rotate(float3(0.0f, 0.0f, 1.0f), rota.z).Normailze().ToMatrix() *
			Translate(trans.x, trans.y, trans.z);
		
}

float4 Yxy2RGB(float Y, float x, float y)
{
	//Yxy to XYZ
	float3 XYZ = float3(x*Y / y, Y, (1.0f - x - y)*Y / y);

	//XYZ to rgb
	return float4(3.240479f*XYZ.x - 1.53715f*XYZ.y - 0.49853f*XYZ.z,
				-0.969256f*XYZ.x + 1.875991f*XYZ.y + 0.041556f*XYZ.z,
				0.055648f*XYZ.x - 0.204043f*XYZ.y + 1.057311f*XYZ.z,
				1.0f);
}
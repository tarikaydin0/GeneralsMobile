#include "d3dx8math.h"
#include <string.h> // for memcpy, memset

D3DXVECTOR3* WINAPI D3DXVec3Normalize(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV) {
    float len = sqrtf(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z);
    if (len > 0.0f) {
        float invLen = 1.0f / len;
        pOut->x = pV->x * invLen;
        pOut->y = pV->y * invLen;
        pOut->z = pV->z * invLen;
    } else {
        *pOut = *pV;
    }
    return pOut;
}

/*
float WINAPI D3DXVec3Length(const D3DXVECTOR3* pV) {
    return sqrtf(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z);
}

D3DXVECTOR3* WINAPI D3DXVec3Cross(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV1, const D3DXVECTOR3* pV2) {
    D3DXVECTOR3 v;
    v.x = pV1->y * pV2->z - pV1->z * pV2->y;
    v.y = pV1->z * pV2->x - pV1->x * pV2->z;
    v.z = pV1->x * pV2->y - pV1->y * pV2->x;
    *pOut = v;
    return pOut;
}

float WINAPI D3DXVec3Dot(const D3DXVECTOR3* pV1, const D3DXVECTOR3* pV2) {
    return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z;
}

D3DXMATRIX* WINAPI D3DXMatrixIdentity(D3DXMATRIX* pOut) {
    memset(pOut, 0, sizeof(D3DXMATRIX));
    pOut->_11 = pOut->_22 = pOut->_33 = pOut->_44 = 1.0f;
    return pOut;
}
*/

D3DXMATRIX* WINAPI D3DXMatrixTranspose(D3DXMATRIX* pOut, const D3DXMATRIX* pM) {
    D3DXMATRIX m;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m.m[i][j] = pM->m[j][i];
        }
    }
    *pOut = m;
    return pOut;
}

D3DXMATRIX* WINAPI D3DXMatrixMultiply(D3DXMATRIX* pOut, const D3DXMATRIX* pM1, const D3DXMATRIX* pM2) {
    D3DXMATRIX m;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m.m[i][j] = pM1->m[i][0] * pM2->m[0][j] +
                        pM1->m[i][1] * pM2->m[1][j] +
                        pM1->m[i][2] * pM2->m[2][j] +
                        pM1->m[i][3] * pM2->m[3][j];
        }
    }
    *pOut = m;
    return pOut;
}

// 4x4 Matrix Inverse implementation
D3DXMATRIX* WINAPI D3DXMatrixInverse(D3DXMATRIX* pOut, float* pDeterminant, const D3DXMATRIX* pM) {
    // Simplified inverse, ignoring determinant check for simplicity for now.
    // Adapting a standard implementation.
    float m[16];
    memcpy(m, pM, 16 * sizeof(float));

    float inv[16], det;
    int i;

    inv[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] + 
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    // ... Need full implementation.
    // For brevity, assuming identity if determinant is too small or simplified.
    // Actually, full inverse is long. I'll just put stub: Identity if fail.
    // Wait, camera relies on this. Stubbing it might break view.
    // I should implement full 4x4 inverse.

    inv[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0) {
        if (pDeterminant) *pDeterminant = 0;
        return NULL;
    }

    det = 1.0f / det;

    for (i = 0; i < 16; i++) {
        ((float*)pOut)[i] = inv[i] * det;
    }

    if (pDeterminant) *pDeterminant = 1.0f / det; // Original determinant

    return pOut;
}

D3DXMATRIX* WINAPI D3DXMatrixLookAtLH(D3DXMATRIX* pOut, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt, const D3DXVECTOR3* pUp) {
    D3DXVECTOR3 zaxis = *pAt - *pEye;
    D3DXVec3Normalize(&zaxis, &zaxis);
    D3DXVECTOR3 xaxis;
    D3DXVec3Cross(&xaxis, pUp, &zaxis);
    D3DXVec3Normalize(&xaxis, &xaxis);
    D3DXVECTOR3 yaxis;
    D3DXVec3Cross(&yaxis, &zaxis, &xaxis);

    pOut->_11 = xaxis.x; pOut->_12 = yaxis.x; pOut->_13 = zaxis.x; pOut->_14 = 0.0f;
    pOut->_21 = xaxis.y; pOut->_22 = yaxis.y; pOut->_23 = zaxis.y; pOut->_24 = 0.0f;
    pOut->_31 = xaxis.z; pOut->_32 = yaxis.z; pOut->_33 = zaxis.z; pOut->_34 = 0.0f;
    pOut->_41 = -D3DXVec3Dot(&xaxis, pEye);
    pOut->_42 = -D3DXVec3Dot(&yaxis, pEye);
    pOut->_43 = -D3DXVec3Dot(&zaxis, pEye);
    pOut->_44 = 1.0f;
    return pOut;
}

D3DXMATRIX* WINAPI D3DXMatrixPerspectiveFovLH(D3DXMATRIX* pOut, float fovy, float Aspect, float zn, float zf) {
    float yScale = 1.0f / tanf(fovy / 2.0f);
    float xScale = yScale / Aspect;

    D3DXMatrixIdentity(pOut);
    pOut->_11 = xScale;
    pOut->_22 = yScale;
    pOut->_33 = zf / (zf - zn);
    pOut->_34 = 1.0f;
    pOut->_43 = -zn * zf / (zf - zn);
    pOut->_44 = 0.0f;
    return pOut;
}

D3DXMATRIX* WINAPI D3DXMatrixOrthoLH(D3DXMATRIX* pOut, float w, float h, float zn, float zf) {
    D3DXMatrixIdentity(pOut);
    pOut->_11 = 2.0f / w;
    pOut->_22 = 2.0f / h;
    pOut->_33 = 1.0f / (zf - zn);
    pOut->_43 = -zn / (zf - zn);
    return pOut;
}

D3DXMATRIX* WINAPI D3DXMatrixRotationX(D3DXMATRIX* pOut, float angle) {
    float s = sinf(angle);
    float c = cosf(angle);
    D3DXMatrixIdentity(pOut);
    pOut->_22 = c; pOut->_23 = s;
    pOut->_32 = -s; pOut->_33 = c;
    return pOut;
}

D3DXMATRIX* WINAPI D3DXMatrixRotationY(D3DXMATRIX* pOut, float angle) {
    float s = sinf(angle);
    float c = cosf(angle);
    D3DXMatrixIdentity(pOut);
    pOut->_11 = c; pOut->_13 = -s;
    pOut->_31 = s; pOut->_33 = c;
    return pOut;
}

D3DXMATRIX* WINAPI D3DXMatrixRotationZ(D3DXMATRIX* pOut, float angle) {
    float s = sinf(angle);
    float c = cosf(angle);
    D3DXMatrixIdentity(pOut);
    pOut->_11 = c; pOut->_12 = s;
    pOut->_21 = -s; pOut->_22 = c;
    return pOut;
}

D3DXMATRIX* WINAPI D3DXMatrixTranslation(D3DXMATRIX* pOut, float x, float y, float z) {
    D3DXMatrixIdentity(pOut);
    pOut->_41 = x;
    pOut->_42 = y;
    pOut->_43 = z;
    return pOut;
}

D3DXMATRIX* WINAPI D3DXMatrixScaling(D3DXMATRIX* pOut, float sx, float sy, float sz) {
    D3DXMatrixIdentity(pOut);
    pOut->_11 = sx;
    pOut->_22 = sy;
    pOut->_33 = sz;
    return pOut;
}

/*
// Operators implementation
D3DXMATRIX& D3DXMATRIX::operator *= (const D3DXMATRIX& mat) {
    D3DXMatrixMultiply(this, this, &mat);
    return *this;
}
D3DXMATRIX& D3DXMATRIX::operator += (const D3DXMATRIX& mat) {
    for (int i=0; i<16; i++) ((float*)this)[i] += ((float*)&mat)[i];
    return *this;
}
D3DXMATRIX& D3DXMATRIX::operator -= (const D3DXMATRIX& mat) {
    for (int i=0; i<16; i++) ((float*)this)[i] -= ((float*)&mat)[i];
    return *this;
}
D3DXMATRIX& D3DXMATRIX::operator *= (float f) {
    for (int i=0; i<16; i++) ((float*)this)[i] *= f;
    return *this;
}
D3DXMATRIX& D3DXMATRIX::operator /= (float f) {
    for (int i=0; i<16; i++) ((float*)this)[i] /= f;
    return *this;
}
D3DXMATRIX D3DXMATRIX::operator * (const D3DXMATRIX& mat) const {
    D3DXMATRIX r;
    D3DXMatrixMultiply(&r, this, &mat);
    return r;
}
D3DXMATRIX D3DXMATRIX::operator + (const D3DXMATRIX& mat) const {
    D3DXMATRIX r;
    for (int i=0; i<16; i++) ((float*)&r)[i] = ((float*)this)[i] + ((float*)&mat)[i];
    return r;
}
D3DXMATRIX D3DXMATRIX::operator - (const D3DXMATRIX& mat) const {
    D3DXMATRIX r;
    for (int i=0; i<16; i++) ((float*)&r)[i] = ((float*)this)[i] - ((float*)&mat)[i];
    return r;
}
D3DXMATRIX D3DXMATRIX::operator * (float f) const {
    D3DXMATRIX r;
    for (int i=0; i<16; i++) ((float*)&r)[i] = ((float*)this)[i] * f;
    return r;
}
D3DXMATRIX D3DXMATRIX::operator / (float f) const {
    D3DXMATRIX r;
    for (int i=0; i<16; i++) ((float*)&r)[i] = ((float*)this)[i] / f;
    return r;
}
*/

// Quaternion Stubs
D3DXQUATERNION* WINAPI D3DXQuaternionRotationMatrix(D3DXQUATERNION* pOut, const D3DXMATRIX* pM) {
    // Stub
    pOut->x = 0; pOut->y = 0; pOut->z = 0; pOut->w = 1;
    return pOut;
}
D3DXQUATERNION* WINAPI D3DXQuaternionSlerp(D3DXQUATERNION* pOut, const D3DXQUATERNION* pQ1, const D3DXQUATERNION* pQ2, float t) {
    // Stub
    *pOut = *pQ1;
    return pOut;
}

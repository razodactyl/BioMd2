#pragma once

#pragma pack(push, 1)

typedef struct tagMd2Header {
	ULONG FileSize;		// 0x00	// Size of (*.md2) file
	ULONG nObject;		// 0x04	// Object Count
} Md2Header;
typedef struct tagMd2Index {
	ULONG pVertice;		// 0x00	// Vertice Pointer (relative)
	ULONG pNormal;		// 0x04	// Normal Pointer (relative)
	ULONG nVector;		// 0x08	// Vector Count
	ULONG pTriangle;	// 0x0C	// Triangle Primitive Pointer (relative)
	ULONG pQuadrangle;	// 0x10	// Quadrangle Primitive Pointer (relative)
	USHORT nTriangle;	// 0x14	// Triangle Count
	USHORT nQuadrangle;	// 0x16	// Quadrangle Count
} Md2Index;

typedef struct tagMd2Vector {
	SHORT x;
	SHORT y;
	SHORT z;
	USHORT padd;
} Md2Vector;

typedef struct tagMd2Triangle {
	UCHAR tu0;
	UCHAR tv0;
	USHORT Clut;
	UCHAR tu1;
	UCHAR tv1;
	UCHAR Page;
	UCHAR vn0;
	UCHAR tu2;
	UCHAR tv2;
	UCHAR vn1;
	UCHAR vn2;
} Md2Triangle;
typedef struct tagMd2Quadrangle {
	UCHAR tu0;
	UCHAR tv0;
	USHORT Clut;
	UCHAR tu1;
	UCHAR tv1;
	USHORT Page;
	UCHAR tu2;
	UCHAR tv2;
	UCHAR vn0;
	UCHAR vn1;
	UCHAR tu3;
	UCHAR tv3;
	UCHAR vn2;
	UCHAR vn3;
} Md2Quadrangle;

typedef struct tagMd2Object {
	std::vector <Md2Vector> v;		// Vertice
	std::vector <Md2Vector> n;		// Normal
	std::vector <Md2Triangle> t;	// Triangle
	std::vector <Md2Quadrangle> q;	// Quadrangle
} Md2Object;

typedef struct tagRESIDENT_EVIL_3_MODEL {
	std::vector <Md2Object> obj;	// Object
	ULONG nVertice;					// Vertice Total Count
	ULONG nNormal;					// Normal Total Count
	ULONG nTriangle;				// Triangle Total Count
	ULONG nQuadrangle;				// Quadrangle Total Count
} RESIDENT_EVIL_3_MODEL;

#pragma pack(pop)

class Resident_Evil_3_Model {
private:
public:

	System_File * File;

	Resident_Evil_3_Model(VOID) {}
	virtual ~Resident_Evil_3_Model(VOID) {}

	RESIDENT_EVIL_3_MODEL Dummy(VOID);

	RESIDENT_EVIL_3_MODEL Read(_iobuf * FILE, ULONG _Offset);
	BOOL Write(RESIDENT_EVIL_3_MODEL Model, _iobuf * FILE, ULONG _Offset);

	RESIDENT_EVIL_3_MODEL Open(CONST CHAR * _FileName, ...);
	BOOL SaveAs(RESIDENT_EVIL_3_MODEL Model, CONST CHAR * _FileName, ...);

	BOOL SaveObjAs(RESIDENT_EVIL_3_MODEL Model, size_t Id, CONST CHAR * _FileName, ...);
	BOOL SaveAllObjs(RESIDENT_EVIL_3_MODEL Model, CONST CHAR * NameBase, CONST CHAR * _Directory, ...);

};
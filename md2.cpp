
#include "stdafx.h"
#include FRAMEWORK_H

#include "md2.h"

// 
RESIDENT_EVIL_3_MODEL Resident_Evil_3_Model::Dummy(VOID) {

	RESIDENT_EVIL_3_MODEL Model;
	Model.nVertice = NULL;
	Model.nNormal = NULL;
	Model.nTriangle = NULL;
	Model.nQuadrangle = NULL;
	if (!Model.obj.empty()) {
		Model.obj.clear();
	}

	return Model;
}
// 
RESIDENT_EVIL_3_MODEL Resident_Evil_3_Model::Read(_iobuf * FILE, ULONG _Offset) {

	// Error
	if (!File->IsOpen(FILE)) { return Dummy(); }

	RESIDENT_EVIL_3_MODEL Model = Dummy();
	Md2Header Header = { NULL };
	Md2Index Index = { NULL };
	Md2Vector Vector = { NULL };
	Md2Triangle Triangle = { NULL };
	Md2Quadrangle Quadrangle = { NULL };
	Md2Object Object;
	ULONG pIndex = _Offset + sizeof(Md2Header);
	ULONG pVertice = NULL;
	ULONG pNormal = NULL;
	ULONG pPrimitive = NULL;
	ULONG pTexture = NULL;

	// Read
	File->Read(FILE, _Offset, &Header, sizeof(Md2Header));
	for (ULONG i = 0; i < Header.nObject; i++, pIndex += sizeof(Md2Index)) {

		// Index
		File->Read(FILE, pIndex, &Index, sizeof(Md2Index));
		Model.obj.push_back(Object);

		// Triangle
		Model.nTriangle += Index.nTriangle;
		pPrimitive = Index.pTriangle + (_Offset + sizeof(Md2Header));
		for (ULONG x = 0; x < Index.nTriangle; x++, pPrimitive += sizeof(Md2Triangle)) {
			File->Read(FILE, pPrimitive, &Triangle, sizeof(Md2Triangle));
			Model.obj[i].t.push_back(Triangle);
		}

		// Quadrangle
		Model.nQuadrangle += Index.nQuadrangle;
		pPrimitive = Index.pQuadrangle + (_Offset + sizeof(Md2Header));
		for (ULONG x = 0; x < Index.nQuadrangle; x++, pPrimitive += sizeof(Md2Quadrangle)) {
			File->Read(FILE, pPrimitive, &Quadrangle, sizeof(Md2Quadrangle));
			Model.obj[i].q.push_back(Quadrangle);
		}

		// Vertice
		Model.nVertice += Index.nVector;
		pVertice = Index.pVertice + (_Offset + sizeof(Md2Header));
		for (ULONG x = 0; x < Index.nVector; x++, pVertice += sizeof(Md2Vector)) {
			File->Read(FILE, pVertice, &Vector, sizeof(Md2Vector));
			Model.obj[i].v.push_back(Vector);
		}

		// Normal
		Model.nNormal += Index.nVector;
		pNormal = Index.pNormal + (_Offset + sizeof(Md2Header));
		for (ULONG x = 0; x < Index.nVector; x++, pNormal += sizeof(Md2Vector)) {
			File->Read(FILE, pNormal, &Vector, sizeof(Md2Vector));
			Model.obj[i].n.push_back(Vector);
		}

	}

	// Terminate
	return Model;
}
BOOL Resident_Evil_3_Model::Write(RESIDENT_EVIL_3_MODEL Model, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!File->IsOpen(FILE)) { return FAIL; }

	// Buffer
	ULONG pIndex = _Offset + sizeof(Md2Header);
	ULONG pPrimitive = pIndex + (sizeof(Md2Index) * Model.obj.size());
	ULONG pVertice = pPrimitive + (sizeof(Md2Triangle) * Model.nTriangle) + (sizeof(Md2Quadrangle) * Model.nQuadrangle);
	ULONG pNormal = pVertice + (sizeof(Md2Vector) * Model.nVertice);

	// Header
	Md2Header Header = { NULL };
	Header.FileSize = NULL;
	Header.nObject = Model.obj.size();
	Md2Index Index = { NULL };

	// Write
	for (size_t i = 0; i < Model.obj.size(); i++, pIndex += sizeof(Md2Index)) {

		// Triangle
		Index.pTriangle = ((pPrimitive - sizeof(Md2Header)) - _Offset);
		Index.nTriangle = Model.obj[i].t.size();
		Header.FileSize += (Model.obj[i].t.size() * sizeof(Md2Triangle));
		for (size_t x = 0; x < Model.obj[i].t.size(); x++, pPrimitive += sizeof(Md2Triangle)) {
			File->Write(FILE, pPrimitive, &Model.obj[i].t[x], sizeof(Md2Triangle));
		}

		// Quadrangle
		Index.pQuadrangle = ((pPrimitive - sizeof(Md2Header)) - _Offset);
		Index.nQuadrangle = Model.obj[i].q.size();
		Header.FileSize += (Model.obj[i].q.size() * sizeof(Md2Quadrangle));
		for (size_t x = 0; x < Model.obj[i].q.size(); x++, pPrimitive += sizeof(Md2Quadrangle)) {
			File->Write(FILE, pPrimitive, &Model.obj[i].q[x], sizeof(Md2Quadrangle));
		}

		// Vertice
		Index.pVertice = ((pVertice - sizeof(Md2Header)) - _Offset);
		Index.nVector = Model.obj[i].v.size();
		File->Write(FILE, pVertice, &Model.obj[i].v[0], (Model.obj[i].v.size() * sizeof(Md2Vector)));
		pVertice += (Model.obj[i].v.size() * sizeof(Md2Vector));
		Header.FileSize += (Model.obj[i].v.size() * sizeof(Md2Vector));

		// Normal
		Index.pNormal = ((pNormal - sizeof(Md2Header)) - _Offset);
		File->Write(FILE, pNormal, &Model.obj[i].n[0], (Model.obj[i].n.size() * sizeof(Md2Vector)));
		pNormal += (Model.obj[i].n.size() * sizeof(Md2Vector));
		Header.FileSize += (Model.obj[i].n.size() * sizeof(Md2Vector));

		// Index
		File->Write(FILE, pIndex, &Index, sizeof(Md2Index));
		Header.FileSize += sizeof(Md2Index);

	}

	// Header
	Header.FileSize += sizeof(Md2Header);
	File->Write(FILE, _Offset, &Header, sizeof(Md2Header));

	// Terminate
	return SUCCESS;
}
// 
RESIDENT_EVIL_3_MODEL Resident_Evil_3_Model::Open(CONST CHAR * _FileName, ...) {

	// _FileName
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _FileName);
	size_t _StrLen = (_vscprintf(_FileName, _ArgList) + 2);
	CHAR * FileName = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(FileName, (_StrLen * 2));
	vsprintf_s(FileName, _StrLen, _FileName, _ArgList);
	__crt_va_end(_ArgList);

	// Error
	if (!File->Exists(FileName)) { return Dummy(); }

	// Open
	_iobuf * _File = File->Open(READ_FILE, FileName);
	if (!_File) { return Dummy(); }

	// Read
	RESIDENT_EVIL_3_MODEL Model = Read(_File, 0x00);

	// Close
	fclose(_File);

	// Terminate
	return Model;
}
BOOL Resident_Evil_3_Model::SaveAs(RESIDENT_EVIL_3_MODEL Model, CONST CHAR * _FileName, ...) {

	// _FileName
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _FileName);
	size_t _StrLen = (_vscprintf(_FileName, _ArgList) + 2);
	CHAR * FileName = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(FileName, (_StrLen * 2));
	vsprintf_s(FileName, _StrLen, _FileName, _ArgList);
	__crt_va_end(_ArgList);

	// Open
	_iobuf * _File = File->Open(CREATE_FILE, FileName);
	if (!_File) { return FAIL; }

	// Write
	BOOL bRet = Write(Model, _File, NULL);

	// Close
	fclose(_File);

	// Terminate
	return bRet;
}
BOOL Resident_Evil_3_Model::SaveObjAs(RESIDENT_EVIL_3_MODEL Model, size_t Id, CONST CHAR * _FileName, ...) {

	// Error
	if ((Id + 1) > Model.obj.size()) { return FAIL; }

	// _FileName
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _FileName);
	size_t _StrLen = (_vscprintf(_FileName, _ArgList) + 2);
	CHAR * FileName = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(FileName, (_StrLen * 2));
	vsprintf_s(FileName, _StrLen, _FileName, _ArgList);
	__crt_va_end(_ArgList);

	// Open
	_iobuf * _File = File->Open(CREATE_FILE, FileName);
	if (!_File) { return FAIL; }

	// Header
	Md2Header Header = { NULL };
	Header.FileSize = NULL;
	Header.nObject = 1;

	// Index
	Md2Index Index = { NULL };
	Index.nVector = Model.obj[Id].v.size();
	Index.nTriangle = Model.obj[Id].t.size();
	Index.nQuadrangle = Model.obj[Id].q.size();
	Index.pTriangle = sizeof(Md2Index);
	Index.pQuadrangle = Index.pTriangle + (sizeof(Md2Triangle) * Model.obj[Id].t.size());
	Index.pVertice = Index.pQuadrangle + (sizeof(Md2Quadrangle) * Model.obj[Id].q.size());
	Index.pNormal = (Index.pVertice + (sizeof(Md2Vector) * Model.obj[Id].v.size()));
	File->Write(_File, sizeof(Md2Header), &Index, sizeof(Md2Index));
	Header.FileSize += sizeof(Md2Index);

	// Triangle
	Header.FileSize += (Model.obj[Id].t.size() * sizeof(Md2Triangle));
	for (size_t x = 0; x < Model.obj[Id].t.size(); x++, Index.pTriangle += sizeof(Md2Triangle)) {
		File->Write(_File, Index.pTriangle + sizeof(Md2Header), &Model.obj[Id].t[0], sizeof(Md2Triangle));
	}

	// Quadrangle
	Header.FileSize += (Model.obj[Id].q.size() * sizeof(Md2Quadrangle));
	for (size_t x = 0; x < Model.obj[Id].q.size(); x++, Index.pQuadrangle += sizeof(Md2Quadrangle)) {
		File->Write(_File, Index.pQuadrangle + sizeof(Md2Header), &Model.obj[Id].q[0], sizeof(Md2Quadrangle));
	}

	// Vertice
	File->Write(_File, Index.pVertice + sizeof(Md2Header), &Model.obj[Id].v[0], (Model.obj[Id].v.size() * sizeof(Md2Vector)));
	Header.FileSize += (Model.obj[Id].v.size() * sizeof(Md2Vector));

	// Normal
	File->Write(_File, Index.pNormal + sizeof(Md2Header), &Model.obj[Id].n[0], (Model.obj[Id].n.size() * sizeof(Md2Vector)));
	Header.FileSize += (Model.obj[Id].n.size() * sizeof(Md2Vector));

	// Header
	Header.FileSize += sizeof(Md2Header);
	File->Write(_File, NULL, &Header, sizeof(Md2Header));

	// Close
	fclose(_File);

	// Terminate
	return SUCCESS;
}
BOOL Resident_Evil_3_Model::SaveAllObjs(RESIDENT_EVIL_3_MODEL Model, CONST CHAR * NameBase, CONST CHAR * _Directory, ...) {

	// _Directory
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _Directory);
	size_t _StrLen = (_vscprintf(_Directory, _ArgList) + 2);
	CHAR * Directory = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(Directory, (_StrLen * 2));
	vsprintf_s(Directory, _StrLen, _Directory, _ArgList);
	__crt_va_end(_ArgList);

	File->CreateDir(Directory);

	if (!strlen(NameBase)) { NameBase = "obj"; }

	// Extract
	BOOL bRet = NULL;
	for (size_t Id = 0; Id < Model.obj.size(); Id++) {
		bRet = SaveObjAs(Model, Id, "%s\\%s%02d.md2", Directory, NameBase, Id);
	}

	// Terminate
	return bRet;
}
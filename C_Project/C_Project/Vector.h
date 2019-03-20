#pragma once

#define ulint unsigned long int
#define uint unsigned int

typedef struct Vector Vector;

struct Vector
{
	ulint	_MaxElements;
	uint	_ElementSize;
	uint	_IncrementialSize;
	ulint	Size;
	void *	_Data;
	
	int (*CreateCustom)(Vector * _this, uint elementSize, ulint initialSize, uint _IncrementialSize);
	int (*Create)(Vector * _this, uint elementSize);
	void (*Free)(Vector * _this);
	int (*Copy)(Vector * _this, struct Vector * source);
	void (*PushBack)(Vector * _this, const void * element);
	void *(*At)(Vector * _this, uint index);
	void (*Clear)(Vector * _this);
	void (*Swap)(Vector * _this, uint index1, uint index2);
};

Vector NewVector();
int CreateVectorCustom(Vector * vec, uint elementSize, ulint initialSize, uint incrementialSize);
int CreateVector(Vector * vec, uint elementSize);
void ReleaseVector(Vector * vec);
int CopyVector(Vector * destVec, Vector * sourceVec);
void VectorPushBack(Vector * vec, const void * element);
void * ElementInVectorAt(Vector * vec, uint index);
void _ExpandVector(Vector * vec);
void ClearVector(Vector * vec);
void Swap(Vector * vec, uint index1, uint index2);

inline Vector NewVector()
{
	Vector v;
	memset(&v, 0, sizeof(v));

	v.CreateCustom = &CreateVectorCustom;
	v.Create = &CreateVector;
	v.Free = &ReleaseVector;
	v.Copy = &CopyVector;
	v.PushBack = &VectorPushBack;
	v.At = &ElementInVectorAt;
	v.Clear = &ClearVector;
	v.Swap = &Swap;

	return v;
}

inline int CreateVectorCustom(Vector * vec, uint elementSize, ulint initialSize, uint incrementialSize)
{
	int succeeded = 0;
	if (vec != NULL && elementSize > 0)
	{
		initialSize = max(initialSize, 1);
		incrementialSize = max(incrementialSize, 1);

		succeeded = 1;
		ReleaseVector(vec);
		vec->Size = 0;
		vec->_ElementSize = elementSize;
		vec->_MaxElements = initialSize;
		vec->_IncrementialSize = incrementialSize;
		vec->_Data = malloc(initialSize * elementSize);
		memset(vec->_Data, 0, initialSize * elementSize);
	}

	return succeeded;
}

inline void ReleaseVector(Vector * vec)
{
	if (vec->_MaxElements > 0)
		free(vec->_Data);
	vec->_ElementSize = 0;
	vec->_MaxElements = 0;
	vec->_IncrementialSize = 0;
	vec->_Data = NULL;
	vec->Size = 0;
}

inline int CreateVector(Vector * vec, uint elementSize)
{
	return CreateVectorCustom(vec, elementSize, 15, 50);
}

inline int CopyVector(Vector * destVec, Vector * sourceVec)
{
	int succeeded = 0;
	if (destVec != NULL && sourceVec != NULL)
	{
		succeeded = 1;
		ReleaseVector(destVec);

		destVec->Size = sourceVec->Size;
		destVec->_ElementSize = sourceVec->_ElementSize;
		destVec->_IncrementialSize = sourceVec->_IncrementialSize;
		destVec->_MaxElements = sourceVec->_MaxElements;

		destVec->_Data = malloc(destVec->_ElementSize * destVec->_MaxElements);
		memcpy(destVec->_Data, sourceVec->_Data, destVec->_ElementSize * destVec->_MaxElements);
	}

	return succeeded;
}

inline void VectorPushBack(Vector * vec, const void * element)
{
	if (!(vec->_MaxElements - vec->Size))
	{
		_ExpandVector(vec);
		vec->_IncrementialSize *= 2;
	}

	memcpy((char*)vec->_Data + vec->Size++ * vec->_ElementSize, element, vec->_ElementSize);
}

inline void * ElementInVectorAt(Vector * vec, uint index)
{
	return ((char*)vec->_Data + index * vec->_ElementSize);
}

inline void _ExpandVector(Vector * vec)
{
	vec->_MaxElements += vec->_IncrementialSize;
	vec->_Data = realloc(vec->_Data, vec->_MaxElements * vec->_ElementSize);
}

inline void ClearVector(Vector * vec)
{
	vec->Size = 0;
}

inline void Swap(Vector * vec, uint index1, uint index2)
{
	void * tmp = malloc(vec->_ElementSize);	
	memcpy(tmp, (char*)vec->_Data + index1 * vec->_ElementSize, vec->_ElementSize);
	memcpy((char*)vec->_Data + index1 * vec->_ElementSize, (char*)vec->_Data + index2 * vec->_ElementSize, vec->_ElementSize);
	memcpy((char*)vec->_Data + index2 * vec->_ElementSize, tmp, vec->_ElementSize);
	free(tmp);
}

#pragma once

#ifdef _ENABLE_PROPERTY_SCRIPT

enum enVarType {
	NULL_VAR = 0, INT_VAR, FLOAT_VAR,
	STRING_VAR, VECTOR_VAR
};


#include"Vector.h"


class CVariable
{
public:
	CVariable() : type(0), floatVal(0), intVal(0), stringVal(0)
	{
		name[0] = '\0';
		vecVal.x = vecVal.y = vecVal.z = 0;
	}
	~CVariable()
	{
		if (stringVal) delete[] stringVal; stringVal = 0;
	}

	void SetData(int t, char* n, void *data);
	void SetData(int t, void *data);

	char *GetName() { return name; }
	int GetType() { return type; }
	int GetDataAsInt() { return intVal; }
	float GetDataAsFloat() { return floatVal; }
	char *GetDataAsString() { return stringVal; }
	CVector3 GetDataAsVector() { return vecVal; }

private:
	char name[128];
	int type;

	int intVal;
	float floatVal;
	char *stringVal;
	CVector3 vecVal;
};


class CPropertyScript
{
public:
	CPropertyScript();
	~CPropertyScript();

	bool IncreaseVariableList();
	bool LoadScriptFile(char *filename);

private:
	void ParseNext(char *tempLine, char *varName);

public:
	bool AddVariable(char *name, int t, void *val);
	bool SetVariable(char *name, int t, void *val);
	int GetVariableAsInt(char *name);
	float GetVariableAsFloat(char *name);
	char *GetVariableAsString(char *name);
	CVector3 GetVariableAsVector(char *name);

	void Shutdown();

private:
	CVariable *variableList;
	int m_totalVars;
	int currentLineChar;
};
#endif // !_ENABLE_PROPERTY_SCRIPT

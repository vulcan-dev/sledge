using System.Numerics;
using System.Runtime.InteropServices;


public delegate void dVoid();

public delegate bool dGetBool();
public delegate void dSetBool(bool bValue);
public delegate bool dGetBoolEntity(uint iHandle);
public delegate void dSetBoolEntity(uint iHandle, bool bValue);


public delegate uint dGetUInt();
public delegate void dSetUInt(uint iValue);
public delegate uint dGetUIntEntity(uint iHandle);
public delegate uint dSetUIntEntity(uint iHandle, uint iValue);

public delegate float dGetFloat();
public delegate void dSetFloat(float fValue);
public delegate float dGetFloatEntity(uint iHandle);
public delegate void dSetFloatEntity(uint iHandle, float fValue);

public delegate Vector2 dGetVector2();
public delegate void dSetVector2(Vector2 vValue);
public delegate Vector2 dGetVector2Entity(uint iHandle);
public delegate void dSetVector2Entity(uint iHandle, Vector2 vValue);

public delegate Vector3 dGetVector3();
public delegate void dSetVector3(Vector3 vValue);
public delegate Vector3 dGetVector3Entity(uint iHandle);
public delegate void dSetVector3Entity(uint iHandle, Vector3 vValue);

public delegate Vector4 dGetVector4();
public delegate void dSetVector4(Vector4 vValue);
public delegate Vector4 dGetVector4Entity(uint iHandle);
public delegate Vector4 dSetVector4Entity(uint iHandle, Vector4 vValue);

public delegate Quaternion dGetQuaternion();
public delegate void dSetQuaternion(Quaternion qValue);
public delegate Quaternion dGetQuaternionEntity(uint iHandle);
public delegate void dSetQuaternionEntity(uint iHandle, Quaternion qValue);

public delegate Transform dGetTransform();
public delegate void dSetTransform(Transform tValue);
public delegate Transform dGetTransformEntity(uint iHandle);
public delegate void dSetTransformEntity(uint iHandle, Transform tValue);
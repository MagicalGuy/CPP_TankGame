#pragma once

class CBaseObject
{
public:
	CBaseObject();
	virtual ~CBaseObject();
	virtual bool DrawObject() = 0;		//绘制对象;
	virtual bool ClsObject() = 0;		//擦除对象;
};
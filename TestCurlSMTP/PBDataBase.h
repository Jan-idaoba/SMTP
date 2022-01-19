#pragma once


class IDatabaseBussiness
{
public:
	virtual ~IDatabaseBussiness() {}


public:
	virtual long DWRetrieve(...) = 0;
	virtual long DWReselectRow(...) = 0;
	virtual long DWUpdate(...);


	virtual long ESQLSelect(...) = 0;
	virtual long ESQLInsert(...) = 0;
	virtual long ESQLDelete(...) = 0;
	virtual long ESQLUpdate(...) = 0;

	virtual long ESQLExecute(...) = 0;
	virtual long ESQLExecuteDyn(...) = 0;
	virtual long ESQLExecuteImmed(...) = 0;
	virtual long ESQLExecuteWithDesc(...) = 0;


	//...........................
};
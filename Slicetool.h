#pragma once


class Slicetool
{
public:
	static Slicetool& GetInstance()
	{
		static Slicetool instance;
		return instance;
	}

	void Slice();


private:
	Slicetool();
	~Slicetool();


};


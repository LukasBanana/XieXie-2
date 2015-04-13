
// C

struct Object
{
	int __xx__private0;
	int __xx__private1;
	int __xx__private2;
};

struct Vector3
{
	Object	super;
	float	x;
	float	y;
	float	z;
};

struct GameObject
{
	Object		super;
	Vector3*	position;
	Quaternion*	rotation;
};

struct Door
{
	static const int	opened = 1;
	static const int	closed = 0;
	GameObject			super;
	int					__xx__private0;
	int					__xx__private1;
};


// C++

struct Object
{
private:
	int __xx__private0;
	int __xx__private1;
	int __xx__private2;
};

struct Vector3 : public Object
{
	float x;
	float y;
	float z;
};

struct GameObject : public Object
{
	Vector3*	position;
	Quaternion*	rotation;
};

struct Door : public GameObject
{
	static const int	opened = 1;
	static const int	closed = 0;
private:
	int					__xx__private0;
	int					__xx__private1;
};


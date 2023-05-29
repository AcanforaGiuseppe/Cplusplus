/* Composite */
#include <vector>

class BoxCollider;
class CircleCollider;
class CompoundCollider;

class IColliderVisitor
{
public:
	virtual void Visit(BoxCollider* Other) = 0;
	virtual void Visit(CircleCollider* Other) = 0;
	virtual void Visit(CompoundCollider* Other) = 0;
};

class ComputeCollisionVisitor : public IColliderVisitor
{
public:
	bool Compute(ICollider* coll1, ICollider* coll2)
	{
		_isSourceDiscovery = true;
		_coll2 = coll2;
		coll1->Accept(this);
		return _isColliding;
	}

	void Visit(BoxCollider* Other)
	{
		if (_isSourceDiscovery)
		{
			_sourceBox = Other;
			_isSourceDiscovery = false;
			_coll2->Accept(this);
		}
		else
		{
			if (_sourceBox)
			{
				// Compute Box to Box collision
				_isColliding = true;
			}
			else if (_sourceCircle)
			{
				// Combute Box to Circle collision
				_isColliding = true;
			}
		}
	}

	void Visit(CircleCollider* Other)
	{
		if (_isSourceDiscovery)
		{
			_sourceCircle = Other;
			_isSourceDiscovery = false;
			_coll2->Accept(this);
		}
		else
		{
			if (_sourceCircle)
			{
				// Compute Circle to Circle collision
				_isColliding = true;
			}
			else if (_sourceBox)
			{
				// Combute Circle to Box collision
				_isColliding = true;
			}
		}

	}

	void Visit(CompoundCollider* Other)
	{
		if (_isSourceDiscovery)
		{
			for (auto* Child : Other->GetChildren())
			{
				_isSourceDiscovery = true;
				_sourceBox = nullptr;
				_sourceCircle = nullptr;
				Child->Accept(this);

				if (_isColliding)
					return;
			}
		}
		else
		{
			for (auto* Child : Other->GetChildren())
			{
				Child->Accept(this);

				if (_isColliding)
					return;
			}
		}
	}

private:
	ICollider* _coll2;
	bool _isSourceDiscovery;
	BoxCollider* _sourceBox;
	CircleCollider* _sourceCircle;
	bool _isColliding;
};


class ICollider
{
public:
	virtual void Accept(IColliderVisitor* Visitor) = 0;
};

class BoxCollider : public ICollider
{
public:
	void Accept(IColliderVisitor* Visitor) override
	{
		return Visitor->Visit(this);
	}
};

class CircleCollider : public ICollider
{
public:
	void Accept(IColliderVisitor* Visitor) override
	{
		return Visitor->Visit(this);
	}
};

class CompoundCollider : public ICollider
{
public:
	void Accept(IColliderVisitor* Visitor) override
	{
		return Visitor->Visit(this);
	}

	void Add(ICollider* collider)
	{
		colliders.push_back(collider);
	}

	const std::vector<ICollider*>& GetChildren() const
	{
		return colliders;
	}

private:
	std::vector<ICollider*> colliders;
};
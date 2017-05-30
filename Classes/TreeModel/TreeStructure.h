#pragma once
#include "TreeCommon.h"

////////////////////////////////////////////////////////////////////

// элемент дерева
class TreeNode
{
public: 
	typedef std::shared_ptr<TreeNode> TreeNodePtr;
protected:
	// откуда вырос элемент
	TreeNode* parent;
	// что выросло из элемента
	std::vector<TreeNodePtr> children;
	
	// внутренности элемента
	TreeElement elem;

public:
	TreeNode() : parent(nullptr) {};

	// установить элементы записи
	void SetInternal(const TreeElement& inter);
	void SetParent(TreeNode* parent);
	void AddChild(const TreeNodePtr child);
	
	//void RemoveChild(int pos);

	// достать элементы записи
	TreeNode* GetParent() const { return parent; }
	TreeNodePtr GetChild(int pos) const { return children.at(pos); }
	const TreeElement& GetInternals() const { return elem; }
	TreeElement& GetInternals() { return elem; }

	int GetChildrenAmount() const;
};

enum AddingResult
{
	ResultUndefined = 0,
	ResultOK = 1,
	ResultNoMoreSlots = 2,
	ResultWrongDirection = 3
};

class TreeInternal
{
public:
	TreeInternal() : root(nullptr) {}
	void Initialize(const Vec2& start_point);

	const AddingResult AddElement(const int branch_id, const Vec2& point_of_grow, int& id, TreePartType tp, double length_coef = 1.0, double width_coef = 1.0);

	void GetElements(std::vector<std::pair<Vec2, Vec2>>& elems, TreePartType tp) const;
	TreeElement& GetElementByID(int& id);
	const TreeElement& GetElementByID(int& id) const;

	void GetGrowPoints(std::vector<std::pair<Vec2, int>>& grow_points) const;

	void GetElementsConsumption(TreeResourceType t, double& val) const;
	void GetElementsProduction(TreeResourceType t, double& val) const;

private:
	std::shared_ptr<TreeNode> root;
	std::map<int, std::shared_ptr<TreeNode>> fast_navigation_map;	
};

class TreeInterface
{
public:
	TreeInterface() : tree(nullptr) {}

	bool MakeTree(const Vec2& start_point);

	// функции для добавления элементов
	const AddingResult AddBranch(const int branch_id, const Vec2& point_of_grow, int& id, double length_coef = 1.0, double width_coef = 0.5);
	const AddingResult AddLeaf(const int branch_id, const Vec2& point_of_grow, int& id);
	const AddingResult AddRoot();

	// функции для получения элементов (заменить на итератор)
	void GetBranches(std::vector<std::pair<Vec2, Vec2>>& branches) const;
	void GetLeafs(std::vector<std::pair<Vec2, Vec2>>& leafs) const;
	void GetRoot(double& current_length) const;

	void GetCurrentProduction(TreeResourceType t, double& val) const;
	void GetCurrentConsumption(TreeResourceType t, double& val) const;

	void GetGrowPoints(std::vector<std::pair<Vec2, int>>& grow_points) const;

	TreeElement& GetElementByID(int& id);
	const TreeElement& GetElementByID(int& id) const;
    
    void Update(float dt) {};

private:
	std::shared_ptr<TreeInternal> tree;
	TreeElement tree_root;
};

using TreePtr = std::shared_ptr<TreeInterface>;

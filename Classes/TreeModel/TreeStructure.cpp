#include "stdafx.h"
#include "TreeStructure.h"
#include "TreeResources.h"

////////////////////////////////////////////////////////////////////

void TreeNode::SetInternal(const TreeElement& inter)
{
	elem = inter;
}

void TreeNode::SetParent(TreeNode* parent_)
{
	parent = parent_;
}

void TreeNode::AddChild(const TreeNodePtr child)
{
	child->SetParent(this);
	children.push_back(child);
}

int TreeNode::GetChildrenAmount() const
{
	return children.size();
	// вынести в отдельную функцию для валидных чайлдов
	/*int res = 0;
	for (const auto& child : children)
	{
		if (child)
			res++;
	}
	return res;*/
}

///////////////

void TreeInternal::Initialize(const Vec2& start_point)
{
	root.reset(new TreeNode());
	TreeElement root_internals;
	root_internals.end_point = start_point;
	root->SetInternal(root_internals);

	fast_navigation_map.clear();
	fast_navigation_map[0] = root;
}

const AddingResult TreeInternal::AddElement(const int branch_id, const Vec2& vec_grow, int& id, TreePartType tp, double length_coef, double width_coef)
{
	auto found = fast_navigation_map.find(branch_id);
	if (found == fast_navigation_map.end())
		return AddingResult::ResultUndefined;

	const TreeElement& old_internals = found->second->GetInternals();

	std::shared_ptr<TreeNode> new_node(new TreeNode());
	TreeElement new_internals;
	new_internals.element_id = fast_navigation_map.size();
	new_internals.start_point = old_internals.end_point;
	new_internals.end_point = new_internals.start_point + vec_grow;
	new_internals.type = tp;

	switch (tp)
	{
	case TreePartType::Branch:
	{
		new_internals.width = width_coef * old_internals.width;
		new_internals.length = (old_internals.length > 0) ? (length_coef * old_internals.length) : length_coef;
		break;
	}
	case TreePartType::Leaf:
	{
		new_internals.width = old_internals.width;
		new_internals.height = new_internals.end_point.y;
		break;
	}
	}

	new_node->SetInternal(new_internals);
	new_node->SetParent(found->second.get());
	
	found->second->AddChild(new_node);

	fast_navigation_map[new_internals.element_id] = new_node;

	id = new_internals.element_id;

	return AddingResult::ResultOK;
}

void TreeInternal::GetElements(std::vector<std::pair<Vec2, Vec2>>& elems, TreePartType tp) const
{
	elems.clear();
	// переделать на обход в глубину
	for (auto i = 0; i != fast_navigation_map.size(); ++i)
	{
		const TreeElement& internals = fast_navigation_map.at(i)->GetInternals();
		if (internals.type == tp)
		{
			elems.push_back(std::make_pair(internals.start_point, internals.end_point));
		}
	}
}

void TreeInternal::GetElementsConsumption(TreeResourceType t, double& val)
{
	double sum = 0;
	for (auto i = 0; i != fast_navigation_map.size(); ++i)
	{
		const TreeElement& internals = fast_navigation_map.at(i)->GetInternals();
		sum += ResourceKeeper::GetConsumption(internals, t);
	}
	val = sum;
}

void TreeInternal::GetElementsProduction(TreeResourceType t, double& val)
{
	double sum = 0;
	for (auto i = 0; i != fast_navigation_map.size(); ++i)
	{
		const TreeElement& internals = fast_navigation_map.at(i)->GetInternals();
		sum += ResourceKeeper::GetProduction(internals, t);
	}
	val = sum;
}

void TreeInternal::GetGrowPoints(std::vector<std::pair<Vec2, int>>& grow_points) const
{
	grow_points.clear();
	// переделать на обход в глубину
	for (auto i = 0; i != fast_navigation_map.size(); ++i)
	{
		const TreeElement& internals = fast_navigation_map.at(i)->GetInternals();
		if (internals.type == TreePartType::Branch)
		{
			grow_points.push_back(std::make_pair(internals.end_point, i));
		}
	}
}

///////////////

bool TreeInterface::MakeTree(const Vec2& start_point)
{
	tree.reset(new TreeInternal());
	tree->Initialize(start_point);

	tree_root.type = TreePartType::Root;
	tree_root.length = 0;
	tree_root.width = 1.0;

	return true;
}

const AddingResult TreeInterface::AddBranch(const int branch_id, const Vec2& point_of_grow, int& id, double length_coef, double width_coef)
{
	return tree->AddElement(branch_id, point_of_grow, id, TreePartType::Branch, length_coef, width_coef);
}

const AddingResult TreeInterface::AddLeaf(const int branch_id, const Vec2& point_of_grow, int& id)
{
	return tree->AddElement(branch_id, point_of_grow, id, TreePartType::Leaf);
}

const AddingResult TreeInterface::AddRoot()
{
	tree_root.length++;
	return AddingResult::ResultOK;
}

void TreeInterface::GetBranches(std::vector<std::pair<Vec2, Vec2>>& branches) const
{
	tree->GetElements(branches, TreePartType::Branch);
}

void TreeInterface::GetLeafs(std::vector<std::pair<Vec2, Vec2>>& leafs) const
{
	tree->GetElements(leafs, TreePartType::Leaf);
}

void TreeInterface::GetCurrentProduction(TreeResourceType t, double& val)
{
	tree->GetElementsProduction(t, val);
	val += ResourceKeeper::GetProduction(tree_root, t);
}

void TreeInterface::GetCurrentConsumption(TreeResourceType t, double& val)
{
	tree->GetElementsConsumption(t, val);
	val += ResourceKeeper::GetConsumption(tree_root, t);
}

void TreeInterface::GetRoot(double& current_length)
{
	current_length = tree_root.length;
}

void TreeInterface::GetGrowPoints(std::vector<std::pair<Vec2, int>>& grow_points) const
{
	tree->GetGrowPoints(grow_points);
}
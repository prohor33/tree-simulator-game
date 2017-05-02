#include "stdafx.h"
#include "Tree.h"

void Tree::add_segment(const TSegment& s) {
    segments_.push_back(s);
}

const std::vector<Tree::TSegment>& Tree::segments() const {
    return segments_;
}

// возвращает точки возможного роста
std::vector<Vec2> Tree::grow_points() const {
    // пока возвращаем тупо концы все веток
    std::vector<Vec2> grow_points;
    grow_points.reserve(segments_.size());
    for (auto& s : segments_)
        grow_points.push_back(s.second);
    return grow_points;
}

// листья
const std::vector<Vec2>& Tree::leafs() const {
    return leafs_;
}

void Tree::add_leaf(const Vec2& leaf) {
    leafs_.push_back(leaf);
}

////////////////////////////////////////////////////////////////////

void TreeNode::SetInternal(const TreeNodeInternal& inter)
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
	TreeNodeInternal root_internals;
	root_internals.end_point = start_point;
	root->SetInternal(root_internals);

	fast_navigation_map.clear();
	fast_navigation_map[0] = root;
}

const AddingResult TreeInternal::AddElement(const int branch_id, const Vec2& vec_grow, int& id, TypeOfPart tp)
{
	auto found = fast_navigation_map.find(branch_id);
	if (found == fast_navigation_map.end())
		return AddingResult::ResultUndefined;

	const TreeNodeInternal& old_internals = found->second->GetInternals();

	std::shared_ptr<TreeNode> new_node(new TreeNode());
	TreeNodeInternal new_internals;
	new_internals.element_id = fast_navigation_map.size();
	new_internals.start_point = old_internals.end_point;
	new_internals.end_point = new_internals.start_point + vec_grow;
	new_internals.type = tp;
	new_internals.width = old_internals.width;

	new_node->SetInternal(new_internals);
	new_node->SetParent(found->second.get());
	
	found->second->AddChild(new_node);

	fast_navigation_map[new_internals.element_id] = new_node;

	id = new_internals.element_id;
}

void TreeInternal::GetElements(std::vector<std::pair<Vec2, Vec2>>& elems, TypeOfPart tp) const
{
	elems.clear();
	// переделать на обход в глубину
	for (auto i = 0; i != fast_navigation_map.size(); ++i)
	{
		const TreeNodeInternal& internals = fast_navigation_map.at(i)->GetInternals();
		if (internals.type == tp)
		{
			elems.push_back(std::make_pair(internals.start_point, internals.end_point));
		}
	}
}

void TreeInternal::GetGrowPoints(std::vector<std::pair<Vec2, int>>& grow_points) const
{
	grow_points.clear();
	// переделать на обход в глубину
	for (auto i = 0; i != fast_navigation_map.size(); ++i)
	{
		const TreeNodeInternal& internals = fast_navigation_map.at(i)->GetInternals();
		if (internals.type == TypeOfPart::Branch)
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

	return true;
}

const AddingResult TreeInterface::AddBranch(const int branch_id, const Vec2& point_of_grow, int& id)
{
	return tree->AddElement(branch_id, point_of_grow, id, TypeOfPart::Branch);
}

const AddingResult TreeInterface::AddLeaf(const int branch_id, const Vec2& point_of_grow, int& id)
{
	return tree->AddElement(branch_id, point_of_grow, id, TypeOfPart::Leaf);
}

void TreeInterface::GetBranches(std::vector<std::pair<Vec2, Vec2>>& branches) const
{
	tree->GetElements(branches, TypeOfPart::Branch);
}

void TreeInterface::GetLeafs(std::vector<std::pair<Vec2, Vec2>>& leafs) const
{
	tree->GetElements(leafs, TypeOfPart::Leaf);
}

void TreeInterface::GetGrowPoints(std::vector<std::pair<Vec2, int>>& grow_points) const
{
	tree->GetGrowPoints(grow_points);
}
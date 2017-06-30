#include "stdafx.h"
#include "TreeStructure.h"
#include "TreeResources.h"

////////////////////////////////////////////////////////////////////

TreeElement dummy_element;

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
    
    // после добавления нового ребенка сортируем их по углу к родителю в порялке против часовой
    {
        auto get_angle_to_parent = [] (const TreeElement& parent, const TreeElement& child) {
            Vec2 v_p = parent.end_point - parent.start_point;
            Vec2 v_c = child.end_point - child.start_point;
            if (v_p.isZero() || v_c.isZero())
                return 0.f;
            return v_p.getAngle(v_c);
        };
        std::sort(children.begin(), children.end(), [&](const TreeNodePtr& a, const TreeNodePtr& b) {
            return get_angle_to_parent(elem, a->elem) < get_angle_to_parent(elem, b->elem);
        });
    }
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
    // хотфикс!
    root_internals.width = 1.f;
	root_internals.end_point = start_point;
	root->SetInternal(root_internals);

	fast_navigation_map.clear();
	fast_navigation_map[0] = root;
}

const AddingResult TreeInternal::AddElement(const int branch_id, const Vec2& vec_grow, int& id, TreePartType tp, double length_coef)
{
	auto found = fast_navigation_map.find(branch_id);
	if (found == fast_navigation_map.end())
		return AddingResult::ResultUndefined;

	const TreeElement& old_internals = found->second->GetInternals();

	std::shared_ptr<TreeNode> new_node(new TreeNode());
	new_node->SetParent(found->second.get());
	found->second->AddChild(new_node);

	TreeElement new_internals;
	new_internals.element_id = fast_navigation_map.size();
	new_internals.start_point = old_internals.end_point;
	new_internals.end_point = new_internals.start_point + vec_grow;
	new_internals.type = tp;

	switch (tp)
	{
	case TreePartType::Branch:
	{
        // ветка всегда добавляется с постоянной толщиной
		new_internals.width = new_branch_width;
		new_internals.length = (old_internals.length > 0) ? (length_coef * old_internals.length) : length_coef;
		break;
	}
	case TreePartType::Leaf:
	{
		new_internals.width = old_internals.width;
		new_internals.height = new_internals.end_point.y;
		double water_consumption = ResourceKeeper::GetConsumption(new_internals, Water);
		new_internals.water_con = water_consumption;

		TreeNode* parent_node = new_node->GetParent();
		while (parent_node != root.get())
		{
			parent_node->GetInternals().water_con += water_consumption;
			parent_node = parent_node->GetParent();
		}
		break;
	}
	}

	new_node->SetInternal(new_internals);
	
	
	

	if (tp == TreePartType::Leaf)
	{
		
	}

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

void TreeInternal::GetElements(std::vector<int>& elems, TreePartType tp) const
{
    elems.clear();
    // переделать на обход в глубину
    for (auto i = 0; i != fast_navigation_map.size(); ++i)
    {
        const TreeElement& internals = fast_navigation_map.at(i)->GetInternals();
        if (internals.type == tp || tp == TreePartType::TypeUndefined)
        {
            elems.push_back(internals.element_id);
        }
    }
}

TreeElement& TreeInternal::GetElementByID(int& id)
{
	auto found = fast_navigation_map.find(id);
	if (found != fast_navigation_map.end())
	{
		return found->second->GetInternals();
	}
	else return dummy_element;
}

const TreeElement& TreeInternal::GetElementByID(int& id) const
{
	auto found = fast_navigation_map.find(id);
	if (found != fast_navigation_map.end())
	{
		return found->second->GetInternals();
	}
	else return dummy_element;
}

void TreeInternal::GetElementsConsumption(TreeResourceType t, double& val) const 
{
	double sum = 0;
	for (auto i = 0; i != fast_navigation_map.size(); ++i)
	{
		const TreeElement& internals = fast_navigation_map.at(i)->GetInternals();
		sum += ResourceKeeper::GetConsumption(internals, t);
	}
	val = sum;
}

void TreeInternal::GetElementsProduction(TreeResourceType t, double& val) const
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

void TreeInternal::GrowTreePartByParameter(float r, std::shared_ptr<TreeNode> branch_start)
{
	Vec2 move_vector;
	branch_start->GetInternals().StretchElement(r, move_vector);
	for (auto i = 0; i != branch_start->GetChildrenAmount(); ++i)
	{
		auto child = branch_start->GetChild(i);
		MoveTreePartOnVector(move_vector, child);
		GrowTreePartByParameter(r, child);
	}
}

void TreeInternal::GrowTreeByParameter(float r)
{
	GrowTreePartByParameter(r, root);
}

void TreeInternal::MoveTreePartOnVector(const Vec2& vec, std::shared_ptr<TreeNode> branch_start)
{
	branch_start->GetInternals().MoveElement(vec);
	for (auto i = 0; i != branch_start->GetChildrenAmount(); ++i)
		MoveTreePartOnVector(vec, branch_start->GetChild(i));
}

double TreeInternal::GetCurrentGlucoseCost(float dt)
{
	double res = 0;
	for (const auto& ent : fast_navigation_map)
	{
		res += ResourceKeeper::GetConsumption(ent.second->GetInternals(), Glucose);
	}
	res *= dt;
	return res;
}

// проходим по всем элементам в глубину
void TreeInternal::DFSIteration(const NodeFunc& before_node_f, const NodeFunc& after_node_f) const
{
    if (before_node_f)
        before_node_f(root);
    DFSIterationImpl(root, before_node_f, after_node_f);
    if (after_node_f)
        after_node_f(root);
}
void TreeInternal::DFSIterationImpl(const TreeNodePtr& node, const NodeFunc& before_node_f, const NodeFunc& after_node_f) const
{
    for (auto& c : node->GetChildren()) {
        if (before_node_f)
            before_node_f(c);
        DFSIterationImpl(c, before_node_f, after_node_f);
        if (after_node_f)
            after_node_f(c);
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

const AddingResult TreeInterface::AddBranch(const int branch_id, const Vec2& point_of_grow, int& id, double length_coef)
{
	return tree->AddElement(branch_id, point_of_grow, id, TreePartType::Branch, length_coef);
}

const AddingResult TreeInterface::AddLeaf(const int branch_id, const Vec2& point_of_grow, int& id)
{
	return tree->AddElement(branch_id, point_of_grow, id, TreePartType::Leaf);
}

const AddingResult TreeInterface::AddRoot()
{
	tree_root.length += 0.2;
	return AddingResult::ResultOK;
}

void TreeInterface::GetBranches(std::vector<std::pair<Vec2, Vec2>>& branches) const
{
	tree->GetElements(branches, TreePartType::Branch);
}

void TreeInterface::GetBranches(std::vector<int>& branches) const
{
    tree->GetElements(branches, TreePartType::Branch);
}

void TreeInterface::GetLeafs(std::vector<std::pair<Vec2, Vec2>>& leafs) const
{
	tree->GetElements(leafs, TreePartType::Leaf);
}

void TreeInterface::GetCurrentProduction(TreeResourceType t, double& val) const
{
	tree->GetElementsProduction(t, val);
	val += ResourceKeeper::GetProduction(tree_root, t);
}

void TreeInterface::GetCurrentConsumption(TreeResourceType t, double& val) const
{
	tree->GetElementsConsumption(t, val);
	val += ResourceKeeper::GetConsumption(tree_root, t);
}

void TreeInterface::GetElementProduction(int id, TreeResourceType t, double& val) const
{
    TreeElement element = GetElementByID(id);
    val = ResourceKeeper::GetProduction(element, t);
}

void TreeInterface::GetElementConsumption(int id, TreeResourceType t, double& val) const
{
    TreeElement element = GetElementByID(id);
    val = ResourceKeeper::GetConsumption(element, t);
}

void TreeInterface::GetRoot(double& current_length) const
{
	current_length = tree_root.length;
}

void TreeInterface::GetElements(std::vector<int>& elements) const
{
    tree->GetElements(elements, TreePartType::TypeUndefined);
}

void TreeInterface::DFSIteration(const NodeFunc& before_node_f, const NodeFunc& after_node_f) const
{
    tree->DFSIteration(before_node_f, after_node_f);
}

void TreeInterface::GetGrowPoints(std::vector<std::pair<Vec2, int>>& grow_points) const
{
	tree->GetGrowPoints(grow_points);
}

TreeElement& TreeInterface::GetElementByID(int& id)
{
	return tree->GetElementByID(id);
}

const TreeElement& TreeInterface::GetElementByID(int& id) const
{
	return tree->GetElementByID(id);
}

const TreeElement& TreeInterface::GetRoot() const
{
    return tree_root;
}

void TreeInterface::Update(float dt)
{
	tree->GrowTreeByParameter(dt * tree_grow_speed);
}

double TreeInterface::GetGlucoseCostForUpdate(float dt)
{
	return tree->GetCurrentGlucoseCost(dt);
}

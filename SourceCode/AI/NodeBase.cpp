#include "Stdafx.h"
#include "NodeBase.h"
#include <magic_enum.hpp>
#include "System/UserFunction.h"

#include "System/Console.h"

NodeBase::~NodeBase()
{
    SafeDelete(judgment);
    SafeDelete(action);
    //delete judgment;
    //delete action;
}

//index�Ԗڂ̎q�m�[�h���擾
NodeBase* NodeBase::GetChild(int index)
{
    if (children.size() > index)
    {
        return nullptr;
    }
    return children.at(index);
}

//�����̎q�m�[�h���擾
NodeBase* NodeBase::GetLastChild()
{
    if (children.size() == 0)
    {
        return nullptr;
    }
    return children.at(children.size() - 1);
}

//�擪�̎q�m�[�h���擾
NodeBase* NodeBase::GetBeginChild()
{
    if (children.size() == 0)
    {
        return nullptr;
    }
    return children.at(0);
}

bool NodeBase::Judgment()
{
    if (judgment != nullptr)
    {
        if (judgment->Judgment())
        {
            return true;
        }
        else
            return false;
    }
    return true;
}

NodeBase* NodeBase::SelectPriority(std::vector<NodeBase*>* list)
{
    NodeBase* selectNode = nullptr;
    int priority = INT_MAX;

    //�D�揇�ʏ��ɏ����𖞂����Ă��邩�m�F���A�������Ă���΂���Action��Ԃ�
    for (int i = 0; i < list->size(); i++)
    {
        for (int j = 0; j < list->at(i)->GetChildSize(); j++)
        {
            if (list->at(i)->children.at(j)->Judgment() == false) continue;
            if (priority > list->at(i)->children.at(j)->GetPriority())
            {
                priority = list->at(i)->children.at(j)->GetPriority();
                selectNode = list->at(i)->children.at(priority - 1);
            }
        }
    }

    return selectNode;
}

NodeBase* NodeBase::SelectAbsolute(std::vector<NodeBase*>* list)
{
    NodeBase* selectNode = nullptr;
    int priority = INT_MAX;

    //�D�揇�ʏ��ɏ����𖞂����Ă��邩�m�F���A�������Ă���΂���Action��Ԃ�
    for (int i = 0; i < list->size(); i++)
    {
        for (int j = 0; j < list->at(i)->GetChildSize(); j++)
        {
            if (list->at(i)->children.at(j)->Judgment() == false) continue;
            if (priority > list->at(i)->children.at(j)->GetPriority())
            {
                if (list->at(i)->rule == BehaviorTree::Rule::Absolute)
                {
                    priority = list->at(i)->children.at(j)->GetPriority();
                    selectNode = list->at(i)->children.at(priority - 1);
                }
            }
        }
    }

    return selectNode;
}

NodeBase* NodeBase::SelectRandom(Enemy* enemy, std::vector<NodeBase*>* list)
{
    //���s���ɋK�����������Ă݂�
#if 0

    //�U���̏ꍇ�͋K���ɉ������s��������
    if ((*list).at(0)->GetParent()->GetName() == "Attack")
    {
        enemy->AddAttackRegulationCount();//�J�E���g��i�߂�
        int selectNo = 0;
        //���̋K���^�C�v�������_���Ɏw��
        if (enemy->GetAttackRegulationCount() <= 0)
        {
            //enemy->AddAttackRegulationCount();//�J�E���g��i�߂�
            selectNo = std::rand() % 4;//0�`3�Ń����_��
            enemy->SetAttackRegulationType(selectNo);
            for (auto& li : *list)
            {
                //���O����v�����s����Ԃ�
                if (li->GetName() == magic_enum::enum_name(enemy->GetStateAttackRegulation()).data())
                {
                    //enemy->AddAttackRegulationCount();//�J�E���g��i�߂�
                    return li;
                }
            }
        }
        else//���ɋK���̃^�C�v�����܂��Ă���ꍇ�i�J�E���g���P�ȏ�̏ꍇ�j
        {          
            for (auto& li : *list)
            {
                //���O����v�����s����Ԃ�
                if (li->GetName() == magic_enum::enum_name(enemy->GetStateAttackRegulation()).data())
                {
                    //enemy->AddAttackRegulationCount();//�J�E���g��i�߂�
                    return li;
                }
            }
        }
    }
    //�U���ȊO�̂Ƃ��̓����_��
    else
#endif 
    {
        //�U���ȊO�ׁ̈A�U���K���̃J�E���g��������
        //enemy->SetAttackRegulationCount(0);
        int selectNo = 0;
        selectNo = std::rand() % list->size();//���X�g�̃T�C�Y���烉���_���l���擾
          //�����_���ɑI�΂ꂽAction��Ԃ�
        return (*list).at(selectNo);
    }
}

NodeBase* NodeBase::SelectSequence(std::vector<NodeBase*>* list, BehaviorData* data)
{
    int step = 0;

    // �w�肳��Ă��钆�ԃm�[�h�̂��V�[�P���X���ǂ��܂Ŏ��s���ꂽ���擾����
    step = data->GetSequenceStep(name);

    // ���ԃm�[�h�ɓo�^����Ă���m�[�h���ȏ�̏ꍇ�A
    if (step >= children.size())
    {
        //���[���ɂ���ď�����؂�ւ���
        // ���[����BehaviorTree::SelectRule::SequentialLooping�̂Ƃ��͍ŏ�������s���邽�߁Astep��0����
        // ���[����BehaviorTree::SelectRule::Sequence�̂Ƃ��͎��Ɏ��s�ł���m�[�h���Ȃ����߁Anullptr�����^�[��
        if (rule == BehaviorTree::Rule::SequenceLooping)
            step = 0;
        else if (rule == BehaviorTree::Rule::Sequence)
            return nullptr;
    }
    // ���s�\���X�g�ɓo�^����Ă���f�[�^�̐��������[�v���s��
    for (auto itr = list->begin(); itr != list->end(); itr++)
    {
        // �q�m�[�h�����s�\���X�g�Ɋ܂܂�Ă��邩
        if (children.at(step)->GetName() == (*itr)->GetName())
        {
            // ���݂̎��s�m�[�h(this)�̕ۑ��A���Ɏ��s����X�e�b�v(step+1)�̕ۑ����s������A
            // ���݂̃X�e�b�v�ԍ��̃m�[�h��Ԃ�
            data->PushSequenceNode(this);
            data->SetSequenceStep(this->GetName(), step + 1);
            return children.at(step);
        }
    }
    // �w�肳�ꂽ���ԃm�[�h�Ɏ��s�\�m�[�h���Ȃ��̂�nullptr�����^�[������
    return nullptr;
}

NodeBase* NodeBase::SearchNode(std::string serchName)
{
    if (name == serchName)
    {
        return this;
    }
    for (auto it = children.begin(); it != children.end(); it++)
    {
        NodeBase* serch = (*it)->SearchNode(serchName);
        if (serch != nullptr)
        {
            return serch;
        }
    }
    return nullptr;
}

NodeBase* NodeBase::Inference(Enemy* enemy, BehaviorData* data)
{
    std::vector<NodeBase*> list;
    NodeBase* result = nullptr;

    for (int i = 0; i < children.size(); i++)
    {
        if (children.at(i)->judgment != nullptr)
        {
            if (children.at(i)->judgment->Judgment())
            {
                list.emplace_back(children.at(i));
            }
        }
        else
        {
            list.emplace_back(children.at(i));
        }
    }

    //���[���ɂ�鏈���̕���
    switch (rule)
    {
    case BehaviorTree::Rule::Priority://�D�揇��
        result = SelectPriority(&list);
        break;
    case BehaviorTree::Rule::Random://�m��
        result = SelectRandom(enemy,&list);
        break;
    case BehaviorTree::Rule::Sequence://�ォ�珇�Ԃ�
    case BehaviorTree::Rule::SequenceLooping://�ォ�珇�ԂɍŌ�܂Ŏ��s�㏉�߂���
        result = SelectSequence(&list,data);
        break;
    case BehaviorTree::Rule::Absolute://�D��I�Ɏ��s
        result = SelectAbsolute(&list);
        break;

    }

    if (result != nullptr)
    {
        if (result->HaveAction())
        {
            return result;
        }
        else
        {
            result = result->Inference(enemy, data);
        }
    }
    return result;
}

NodeBase* NodeBase::InferenceAbsolute(Enemy* enemy, BehaviorData* data)
{
    std::vector<NodeBase*> list;
    NodeBase* result = nullptr;

    for (int i = 0; i < children.size(); i++)
    {
        if (children.at(i)->judgment != nullptr)
        {
            if (children.at(i)->judgment->Judgment() && children.at(i)->rule== BehaviorTree::Rule::Absolute)
            {
                list.emplace_back(children.at(i));
            }
        }
    }
    if (list.size() <= 0)
        return nullptr;
    result = SelectAbsolute(&list);

    if (result != nullptr)
    {
        if (result->HaveAction())
        {
            return result;
        }
        else
        {
            result = result->Inference(enemy, data);
        }
    }
    return result;
}

ActionBase::State NodeBase::Run()
{
    if (action != nullptr)
    {
        //�s�������s����
        return action->Run();
    }

    return ActionBase::State::Failed;
}

//�s���̏�������
void NodeBase::Enter()
{
    if (action != nullptr)
    {
        ConsoleFunc::WriteEndl("Enter");
        action->Enter();
    }
}

//�s���̏I������
void NodeBase::Exit()
{
    if (action != nullptr)
    {
        ConsoleFunc::WriteEndl("Exit");
        action->Exit();
    }
}

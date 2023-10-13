#include "Stdafx.h"
#include "BehaviorData.h"

//�V�[�P���X�m�[�h���|�b�v(�擪�̗v�f���P�폜)
NodeBase* BehaviorData::PopSequenceNode()
{
    //��̏ꍇ��nullptr��Ԃ�
    if (sequenceStack.empty())
        return nullptr;
    NodeBase* node = sequenceStack.top();
    if (node != nullptr)
    {
        //���o�����f�[�^���폜
        sequenceStack.pop();
    }
    return node;
}

//�V�[�P���X�X�e�b�v�̃Q�b�^�[
int BehaviorData::GetSequenceStep(std::string name)
{
    if (runSequenceStepMap.count(name) == 0)
    {
        //�L�[�����݂��Ȃ���Βǉ�
        runSequenceStepMap.insert(std::make_pair(name, 0));

    }
    return runSequenceStepMap.at(name);
}

//�V�[�P���X�X�e�b�v�̃Z�b�^�[
void BehaviorData::SetSequenceStep(std::string name, int step)
{
    runSequenceStepMap.at(name) = step;
}

//������
void BehaviorData::Init()
{
    runSequenceStepMap.clear();
    while (sequenceStack.size() > 0)
    {
        sequenceStack.pop();
    }
}

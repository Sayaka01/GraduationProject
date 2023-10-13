#pragma once

#include <string>
#include <stack>
#include <map>
class NodeBase;

class BehaviorData
{
public:
    //�R���X�g���N�^
    BehaviorData() { Init(); }

    //�V�[�P���X�m�[�h���v�b�V��
    void PushSequenceNode(NodeBase* node) { sequenceStack.push(node); }

    //�V�[�P���X�m�[�h���|�b�v(�擪�̗v�f���P�폜)
    NodeBase* PopSequenceNode();

    //�V�[�P���X�X�e�b�v�̃Q�b�^�[
    int GetSequenceStep(std::string name);

    //�V�[�P���X�X�e�b�v�̃Z�b�^�[
    void SetSequenceStep(std::string name, int step);

    //������
    void Init();

private:
    std::stack<NodeBase*> sequenceStack;//���s���钆�ԃm�[�h���X�^�b�N
    std::map<std::string, int> runSequenceStepMap;//���s���̒��ԃm�[�h�̃X�e�b�v���L�^

};
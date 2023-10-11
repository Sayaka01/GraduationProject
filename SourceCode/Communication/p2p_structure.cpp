#include "Stdafx.h"
#include "p2p_structure.h"
//#include "../../imgui/imgui.h"
#include <ws2tcpip.h>
//#include <magic_enum.hpp>
//#include "hash.h"

HostP2P::~HostP2P()
{
}

// �����ݒ�
bool HostP2P::Initialize(ID3D11Device* device)
{
    PeerToPeerModel::Initialize(device);

    // TCP�T�[�o�[�̋N��
    if (tcpServer.Initialize(portIndex) == false)
        return false;

    // UDP�T�[�o�[�̋N��
    if (udpServer.Initialize(portIndex) == false)
        return false;

    // �}���`�L���X�g�p�T�[�o�[�N��
    if (multicastSender.Initialize(multiPortIndex) == false)
        return false;

    // ����ۑ�
    otherGuestData[0].socketTCP = tcpServer.GetSocket();
    otherGuestData[0].addrTCP = tcpServer.GetSocketAddress();
    otherGuestData[0].addrUDP = ChangeUDPAddress(tcpServer.GetSocketAddress());

    listIndex = 0;
    return true;
}

// �\�P�b�g�̏I������
void HostP2P::Finalize()
{
    for (int i = 0; i < MaxConnectNumber; i++)
    {
        int close = closesocket(otherGuestData[i].socketTCP);
        if (close != 0)
        {
            Fail("�I�������Ɏ��s���܂��� �G���[�R�[�h�F");
        }
    }
}

// �Q�X�g�̓����҂�
void HostP2P::WaitConnectFromGuest()
{
    // �Q�X�g�ɂ��TCP�T�[�o�[�ւ̒ʐM�҂�
    int emptyIndex = GetEmptyIndex();
    ComputerData data{};

    if (tcpServer.AcceptOnSelect(emptyIndex, &data)==true)//�������������ꍇ
    {
        // �Q�X�g�̃f�[�^��ۊ�
        RegisterList(emptyIndex, data);
        tcpServer.SetList(&otherGuestData[emptyIndex].socketTCP);

        // �m�F�̂��߂̃��b�Z�[�W�𑗐M
        ChatCommand chatCom;
        chatCom.actionType = static_cast<char>(ActionCommandType::Chat);
        char chatMessage[128] = { "thank you connecting." };
        strcpy_s(chatCom.message, strlen(chatMessage) + 1, chatMessage);
        udpServer.Send(otherGuestData[emptyIndex].addrUDP, (char*)&chatCom, sizeof(ChatCommand));
      
    }
}

// ���[�U�[�̖��O��o�^,�����ݒ�
bool HostP2P::InitializePassName()
{
    char name[20]{};
    strcpy_s(name, strlen(playerName.c_str()) + 1, playerName.c_str());
    otherGuestData[listIndex].userName = std::string(name);
    return true;
}

// �`���b�g���b�Z�[�W�𑗐M
void HostP2P::SendChatMessage(char chatMessage[128])
{
    ChatCommand chatCom;
    chatCom.actionType = static_cast<char>(ActionCommandType::Chat);
    //chatCom.actionFrame = SceneManager::Instance().GetActionFrame() + ragTime;// 1�b��(10�t���[����)�ɕ\��
    //snprintf(chatCom.message, sizeof(chatMessage), chatMessage);
    strcpy_s(chatCom.message, strlen(chatMessage) + 1, chatMessage);

    for (int i = 1; i < MaxConnectNumber; i++)
    {
        if (otherGuestData[i].addrUDP.sin_addr.S_un.S_addr != 0)
            udpServer.Send(otherGuestData[i].addrUDP,(char*)&chatCom, sizeof(ChatCommand));
    }
}

// �Ȃ��Ă����N���C�A���g�̏���ۑ�
void HostP2P::RegisterList(int index, ComputerData data)
{
    //���ɒʐM�ς��ǂ����m�F����
    int connectIndex = CheckAlreadyConnect(&data.addrTCP);

    if (connectIndex >= 0)// ���ɒʐM�ς̑���ł����
    {
        ConsoleFunc::WriteEndl("���ɓ��o�ςł�");
        return;
    }

    //�V�K�̑���̏ꍇ
    otherGuestData[index] = data;

    ConsoleFunc::Write("�V�K�N���C�A���g��o�^���܂���");
    std::cout << (int)index << std::endl;

    //�Q�����R�}���h�𑗐M
    JoinPermitCommand joinPermitCom;
    joinPermitCom.actionType = (char)ActionCommandType::JoinPermit;
    joinPermitCom.index = index;
    tcpServer.Send(otherGuestData[index].socketTCP, (char*)&joinPermitCom, sizeof(JoinPermitCommand));

    // TCP�̃A�h���X����UDP�̃A�h���X�֕ϊ�
    otherGuestData[index].addrUDP = ChangeUDPAddress(otherGuestData[index].addrTCP);
    
    // �Ȃ��Ă����v���C���[�̃��f����ǉ�
    //SceneManager::Instance().JoinPlayer(index, initialPos[index]);

    // �ʐM���Ă���l�����̖��O�̃��X�g�𑗂�
    //PassNameCommand passName;
    //passName.actionType = static_cast<char>(ActionCommandType::PassName);
    //for (int i = 0; i < MaxConnectNumber; i++)
    //{
    //    char name[20]{};
    //    strcpy_s(name, strlen(otherGuestData[i].userName.c_str()) + 1, otherGuestData[i].userName.c_str());
    //    passName.name[i] = std::string(name);
    //}
    //tcpServer.Send(otherGuestData[index].socketTCP, (char*)&passName, sizeof(PassNameCommand));
}

// �����̒l(IP�A�h���X�A�|�[�g�ԍ�)���炷�łɓo�^���Ă���N���C�A���g�����ׂ�
// �߂�l�i�����̏ꍇ�F�o�^���Ă���index�@�V�K�̏ꍇ�F�[�P�j
int HostP2P::CheckAlreadyConnect(sockaddr_in* addr)
{
    for (int i = 0; i < MaxConnectNumber; i++)
    {
        // �A�h���X�ƃ|�[�g�ԍ�����v���邩���ׂ�
        if (addr->sin_addr.S_un.S_addr == otherGuestData[i].addrUDP.sin_addr.S_un.S_addr &&
            addr->sin_port == otherGuestData[i].addrUDP.sin_port)
            return i;
    }
    return -1;
}

// �f�[�^���󂯎�������̏���
void HostP2P::Recieve()
{
    buffer = nullptr;
    action = -1;
    sockaddr_in udpAddr;
    //if (udpServer.Recieve())
    // UDP�ʐM�Ŏ�M
    udpAddr = udpServer.Recieve();
    if (udpAddr.sin_addr.S_un.S_addr != 0)// ��M�ɐ��������ꍇ
    {
        // �����Ă����R�}���h������
        CommandFromData(udpServer.GetBuffer());

        // ���M�҂̏����󂯎�����ꍇ�͐V�K�o�^�Ɗ����̒ʐM�҂ɑ��M�҂̏��𑗂�
        //if (action == (char)ActionCommandType::GiveOwnData)
        //{
        //    OwnDataCommand* ownDataCom = (OwnDataCommand*)(udpServer.GetBuffer());
        //    int index = ownDataCom->guestIndex;
        //    otherGuestData[index].addrUDP = udpAddr;
        //    otherGuestData[index].userName = ownDataCom->name;
        //    // �p�X���[�h���Ⴄ�ꍇ�@�@�@
        //    if (password != ownDataCom->password)
        //    {
        //        // �ޏo�R�}���h�𑗐M
        //        ExitCommand exitCom;
        //        exitCom.playerIndex = index;
        //        tcpServer.Send(otherGuestData[index].socketTCP, (char*)&exitCom, sizeof(ExitCommand));

        //        // ����̃f�[�^���폜
        //        ClearData(index);
        //    }
        //    else
        //    {                
        //        ChatCommand chatCom;
        //        chatCom.actionType = static_cast<char>(ActionCommandType::Chat);
        //        char chatMessage[128] = { "your udp address add." };
        //        strcpy_s(chatCom.message, strlen(chatMessage) + 1, chatMessage);
        //        // ����o�^�������Ƃ𑊎�ɓ`����
        //        udpServer.Send(otherGuestData[index].addrUDP, (char*)&chatCom, sizeof(ChatCommand));

        //        // ���M�҈ȊO�ɑ��M�҂̓��o��m�点��
        //        for (int i = 1; i < MaxConnectNumber; i++)
        //        {
        //            if (i == index)continue;
        //            if (otherGuestData[i].socketTCP == INVALID_SOCKET)continue;
        //            // �����Q�X�g�ɐV�K�Q�X�g�̑����𑗐M
        //            IncreaseGuestCommand increaseGuestCom;
        //            increaseGuestCom.actionType = (char)ActionCommandType::IncreaseGuest;
        //            increaseGuestCom.index = index;
        //            increaseGuestCom.newGuestData = otherGuestData[index];
        //            tcpServer.Send(otherGuestData[i].socketTCP, (char*)&increaseGuestCom, sizeof(IncreaseGuestCommand));
        //        }
        //    }
        //}

    }

    // TCP�ʐM�Ń\�P�b�g�ɕω����������ꍇ�̂ݎ�M
    for (int i = 0; i < MaxConnectNumber; i++)
    {
        if (otherGuestData[i].socketTCP == INVALID_SOCKET)continue;
        if (tcpServer.RecieveOnSelect(otherGuestData[i].socketTCP))
            CommandFromData(tcpServer.GetBuffer());
    }
}

// �q�����Ă��鑊��S���Ƀf�[�^�𑗐M
void HostP2P::SendAll(char* data,int size)
{
    for (int i = 1; i < MaxConnectNumber; i++)
    {
        if (otherGuestData[i].addrUDP.sin_addr.S_un.S_addr == 0)continue;
        udpServer.Send(otherGuestData[i].addrUDP,data, size);
    }
}

// �����Ă����R�}���h����������
void HostP2P::CommandFromData(char* buff)
{
    buffer = buff;
    action = buffer[0];
    switch (action)
    {
    case static_cast<int>(ActionCommandType::Exit):// �ޏo�v��
        ConsoleFunc::Write("�ޏo�R�}���h(index: ");
        {
            //ExitCommand* recieveExitCom = (ExitCommand*)(buff);
            // �ޏo�R�}���h�𑗂��Ă���PC�̃f�[�^�����݂��Ă����
            //if (otherGuestData[recieveExitCom->playerIndex].socketTCP != INVALID_SOCKET)
            //{
            //    ConsoleFunc::Write((std::to_string(recieveExitCom->playerIndex)+" ):").c_str());
            //    // �o�^�f�[�^���폜
            //    ClearData(recieveExitCom->playerIndex);
            //    SceneManager::Instance().ExitPlayer(recieveExitCom->playerIndex);

            //    // �����Ă����Q�X�g�ɑޏo���R�}���h�𑗂�
            //    ExitCommand exitPermitCom{};
            //    exitPermitCom.actionType = (char)ActionCommandType::ExitPermit;
            //    exitPermitCom.playerIndex = recieveExitCom->playerIndex;
            //    SendTCP((char*)&exitPermitCom, sizeof(ExitCommand), exitPermitCom.playerIndex);
            //    
            //    // �����Ă����Q�X�g�Ƒ��̐ڑ����Ă���Q�X�g�ɑޏo�����Q�X�g�����邱�Ƃ�m�点��
            //    ExitCommand exitNoticeCom{};
            //    exitNoticeCom.actionType = (char)ActionCommandType::ExitNotice;
            //    exitNoticeCom.playerIndex = recieveExitCom->playerIndex;

            //    for (int i = 1; i < MaxConnectNumber; i++)
            //    {
            //        if (otherGuestData[i].socketTCP == INVALID_SOCKET)continue;
            //        SendTCP((char*)&exitNoticeCom, sizeof(ExitCommand), i);
            //    }
            //    ConsoleFunc::WriteEndl("�ޏo��������I��");
            //}
            ConsoleFunc::WriteEndl("���ɑޏo�ς�PC����̎��s");
        }
        break;
    case static_cast<int>(ActionCommandType::Chat):// �`���b�g��M
        //Console::WriteEndl("�`���b�g��M");
        //{
        //    ChatCommand* chatCom = (ChatCommand*)(buff);
        //    Console::Write(chatCom->message);
        //    Console::WriteEndl(std::to_string(chatCom->actionFrame).c_str());
        //    ChatData* chatData = new ChatData(chatCom->message, chatCom->actionFrame);
        //    recieveMessages.emplace_back(chatData);
        //}
        break;
    default:
        ConsoleFunc::Write("��M�����R�}���h�F");
        //ConsoleFunc::WriteEndl(magic_enum::enum_name(static_cast<ActionCommandType>(action)).data());
        break;
    }
    std::cout << std::endl;
}

// TCP�ʐM���g�p���ē���̑���(Index�Ŏw��)�Ƀf�[�^�𑗐M
void HostP2P::SendTCP(char* data, int size, int index)
{
    // �\�P�b�g���o�^����Ă���΃R�}���h�𑗂�
    if (otherGuestData[index].socketTCP == INVALID_SOCKET)return;
    tcpServer.Send(otherGuestData[index].socketTCP, data, size);
}

// �}���`�L���X�g���g�p���ăf�[�^��S���ɑ��M
void HostP2P::SendByMulticast(char* data, int size)
{
    multicastSender.Send(data, size);
}

GuestP2P::~GuestP2P()
{
}

// �����ݒ�
bool GuestP2P::Initialize(ID3D11Device* device)
{
    PeerToPeerModel::Initialize(device);

    // TCP�N���C�A���g���N��
    if (tcpClient.Initialize(portIndex, inputHostAddress) == false)
        return false;
 
    // ���o������M����
    if (tcpClient.Recieve())
        CommandFromData(tcpClient.GetBuffer());

    // UDP�T�[�o�[�̋N��
    if (udpServer.Initialize(portIndex + listIndex) == false)
        return false;

    // �}���`�L���X�g�p�N���C�A���g�N��
    if (!multicastReciever.Initialize(multiPortIndex/*+listIndex*/))
        return false;

    // ���O�̏����󂯎��
    if (tcpClient.Recieve())
        CommandFromData(tcpClient.GetBuffer());

    return true;
}

// �`���b�g���b�Z�[�W���T�[�o�[�֑��M
void GuestP2P::SendChatMessage(char chatMessage[128])
{
    ChatCommand chatCom;
    chatCom.actionType = static_cast<char>(ActionCommandType::Chat);
    //chatCom.actionFrame = SceneManager::Instance().GetActionFrame() + ragTime;// 1�b��(10�t���[����)�ɕ\��
//snprintf(chatCom.message, sizeof(chatMessage), chatMessage);
    strcpy_s(chatCom.message, strlen(chatMessage) + 1, chatMessage);

    for (int i = 0; i < MaxConnectNumber; i++)
    {
        if (otherGuestData[i].addrUDP.sin_addr.S_un.S_addr != 0)
            udpServer.Send(otherGuestData[i].addrUDP,(char*)&chatCom, sizeof(ChatCommand));
    }
}

// ���[�U�[�̖��O��o�^,�����ݒ�
bool GuestP2P::InitializePassName()
{
    char name[20]{};
    strcpy_s(name, strlen(playerName.c_str()) + 1, playerName.c_str());
    otherGuestData[listIndex].userName = std::string(name);

    //Hash(password);
    // ���[�U�[�i�����j�̏����z�X�g�ɑ��M
    //OwnDataCommand ownDataCom;
    //ownDataCom.actionType = static_cast<char>(ActionCommandType::GiveOwnData);
    //ownDataCom.guestIndex = listIndex;
    //ownDataCom.name = otherGuestData[listIndex].userName;
    //ownDataCom.password = password;
    //udpServer.Send(otherGuestData[0].addrUDP, (char*)&ownDataCom, sizeof(OwnDataCommand));
   
    // UDP���g�p���đ��M���ꂽ�f�[�^��M
    sockaddr_in recieveAddr{};
    recieveAddr = udpServer.Recieve();
    if (recieveAddr.sin_addr.S_un.S_addr != 0)
        CommandFromData(udpServer.GetBuffer());//��M�����f�[�^�̏���

    // �z�X�g����TCP���g�p���đ��M���ꂽ�f�[�^����M
    if (tcpClient.Recieve())
    {
        // ��M�����f�[�^�̏���
        CommandFromData(tcpClient.GetBuffer());
        // �ޏo�R�}���h���󂯎������
        if (action == static_cast<char>(ActionCommandType::Exit))
        {
            return false;
        }
    }
    return true;
}

// �f�[�^���󂯎�������̏���
void GuestP2P::Recieve()
{
    buffer = nullptr;
    action = -1;
    if (udpServer.Recieve().sin_addr.S_un.S_addr != 0)
        CommandFromData(udpServer.GetBuffer());
    //for (int i = 0; i < MaxConnectNumber; i++)
    if (tcpClient.RecieveOnSelect())
        CommandFromData(tcpClient.GetBuffer());
    if(multicastReciever.Recieve())
        CommandFromData(multicastReciever.GetBuffer());
}

// �q�����Ă��鑊��S���Ƀf�[�^�𑗐M
void GuestP2P::SendAll(char* data, int size)
{
    for (int i = 0; i < MaxConnectNumber; i++)
    {
        // �f�[�^���Ȃ���Α���Ȃ�
        if (otherGuestData[i].addrUDP.sin_addr.S_un.S_addr == 0)continue;
        // �������g�ɂ͑���Ȃ�
        if (i == listIndex)continue;

        udpServer.Send(otherGuestData[i].addrUDP,data, size);
    }
}

// �����Ă����R�}���h����������
void GuestP2P::CommandFromData(char* buff)
{
    buffer = buff;
    action = buff[0];

    switch (action)
    {
    case static_cast<int>(ActionCommandType::JoinPermit):// ��������
        ConsoleFunc::WriteEndl("���o������M");
        {
            //JoinPermitCommand* joinPermitCom = (JoinPermitCommand*)(buff);
            //listIndex = (int)joinPermitCom->ownIndex;
            //SceneManager::Instance().SetHostStartActionFrame(joinPermitCom->actionFrame);
            std::cout << "�����̎��ʔԍ�:" << listIndex << std::endl;
            for (int i = 0; i < MaxConnectNumber; i++)
            {
                //if (joinPermitCom->preJoinData[i].addrTCP.sin_addr.S_un.S_addr == 0)continue;
                //otherGuestData[i] = joinPermitCom->preJoinData[i];
                std::cout << "�Ȃ����Ă���PC��IPAddress(";
                std::cout << (int)otherGuestData[i].addrTCP.sin_addr.S_un.S_un_b.s_b1 << ".";
                std::cout << (int)otherGuestData[i].addrTCP.sin_addr.S_un.S_un_b.s_b2 << ".";
                std::cout << (int)otherGuestData[i].addrTCP.sin_addr.S_un.S_un_b.s_b3 << ".";
                std::cout << (int)otherGuestData[i].addrTCP.sin_addr.S_un.S_un_b.s_b4 << ")" << std::endl;
                std::cout << "�|�[�g�ԍ�:" << (int)otherGuestData[i].addrTCP.sin_port << std::endl;
                if (otherGuestData[i].addrTCP.sin_addr.S_un.S_addr == 0)continue;
                // �v���C���[�̎Q�������Q�[���V�[���ŏ���
                //if ((int)joinPermitCom->ownIndex != i)
                //    SceneManager::Instance().JoinPlayer(i, TypeConversionShort3::ChangeFloat3Decimal2(joinPermitCom->joinPosition[i]));
            }
        }
        break;
    case static_cast<int>(ActionCommandType::Exit):// �ޏo�v��
        ConsoleFunc::WriteEndl("�ޏo�v������M");
        break;
    case static_cast<int>(ActionCommandType::ExitPermit):// �ޏo����
        ConsoleFunc::WriteEndl("�ޏo������M");
        break;
    case static_cast<int>(ActionCommandType::Chat):// �`���b�g��M
        //Console::WriteEndl("�`���b�g��M");
        //{
        //    ChatCommand* chatCom = (ChatCommand*)(buff);
        //    Console::WriteEndl(chatCom->message);
        //    Console::WriteEndl(std::to_string(chatCom->actionFrame).c_str());
        //    ChatData* chatData = new ChatData(chatCom->message, chatCom->actionFrame);
        //    recieveMessages.emplace_back(chatData);
        //}
        break;
    default:
    {
        ActionCommandType state = static_cast<ActionCommandType>(action);
        ConsoleFunc::Write("�R�}���h�F");
        //ConsoleFunc::WriteEndl(magic_enum::enum_name(state).data());
    }
    break;
    }
    std::cout << std::endl;
}

//TCP���g�p���Ďw��̑���փf�[�^�𑗐M
void GuestP2P::SendTCP(char* data, int size, int index)
{
    tcpClient.Send(data, size);
}

// �w���TCP�A�h���X����UDP�A�h���X���擾
sockaddr_in PeerToPeerModel::ChangeUDPAddress(sockaddr_in srcAddr)
{
    sockaddr_in resultAddr;

    // �N���C�A���g�̎󂯓�����̐ݒ�
    addrinfo* addrInfo = NULL;
    addrinfo tmpAddr;
    ZeroMemory(&tmpAddr, sizeof(addrinfo));
    // �ݒ肷�����ݒ�
    tmpAddr.ai_flags = 0;
    tmpAddr.ai_family = AF_INET;
    tmpAddr.ai_socktype = SOCK_DGRAM;
    tmpAddr.ai_protocol = 0;
    // �A�h���X�̌^�ύX
    std::string strAddress = 
        std::to_string(srcAddr.sin_addr.S_un.S_un_b.s_b1) + "." +
        std::to_string(srcAddr.sin_addr.S_un.S_un_b.s_b2) + "." +
        std::to_string(srcAddr.sin_addr.S_un.S_un_b.s_b3) + "." +
        std::to_string(srcAddr.sin_addr.S_un.S_un_b.s_b4);
    std::string strPort = std::to_string(srcAddr.sin_port);

    if (getaddrinfo(strAddress.c_str(), strPort.c_str(), &tmpAddr, &addrInfo) != 0)
    {
        Fail("�z�X�g�̖��O����A�h���X�̎擾�Ɏ��s���܂���");
        return { 0 };
    }
    resultAddr = *((sockaddr_in*)addrInfo->ai_addr);

    std::cout << "�z�X�g��IPAddress(";
    std::cout << (int)resultAddr.sin_addr.S_un.S_un_b.s_b1 << ".";
    std::cout << (int)resultAddr.sin_addr.S_un.S_un_b.s_b2 << ".";
    std::cout << (int)resultAddr.sin_addr.S_un.S_un_b.s_b3 << ".";
    std::cout << (int)resultAddr.sin_addr.S_un.S_un_b.s_b4 << ")" << std::endl;
    return resultAddr;
}
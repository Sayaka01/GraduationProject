#include "Stdafx.h"
#include "client_server_structure.h"

#include "../System/console.h"
//#include <magic_enum.hpp>

//-------------------------------------------------------------------------------------------------------------
// �N���C�A���g�T�[�o�[�^�@
//-------------------------------------------------------------------------------------------------------------

// �R���X�g���N�^
ClientServerModel::ClientServerModel()
{
    WSADATA data;
    //WSA�̏�����
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
    {
        //���������s
        Fail("WSA�̏������Ɏ��s���܂���");
        return;
    }

    //�f�[�^��������
    for (int i = 0; i < MaxMemberSize; i++)
    {
        ClearData(i);
    }


    ConsoleFunc::OpenWindow();
}

// �f�X�g���N�^
ClientServerModel::~ClientServerModel()
{
    if (multicastSender != nullptr)
        delete multicastSender;
    if (multicastReciever != nullptr)
        delete multicastReciever;
    for (int i = 0; i < MaxMemberSize; i++)
    {
        int close = closesocket(connectMemberList[i].socketTCP);
        if (close != 0)
        {
            Fail("�I�������Ɏ��s���܂��� �G���[�R�[�h�F");
        }
    }
    //WSA�̉��
    WSACleanup();

}

// �R���\�[����ʂɒʐM���̑���̃��X�g��S�ĕ\��
void ClientServerModel::WriteConsoleConnectListAll()
{
    for (int i = 0; i < MaxMemberSize; i++)
    {
        std::cout << "IPAddress���X�g" << i << ":(";
        //std::cout << (int)connectMemberList[i].sin_addr.S_un.S_un_b.s_b1 << ".";
        //std::cout << (int)connectMemberList[i].addrTCP.sin_addr.S_un.S_un_b.s_b2 << ".";
        //std::cout << (int)connectMemberList[i].addrTCP.sin_addr.S_un.S_un_b.s_b3 << ".";
        //std::cout << (int)connectMemberList[i].addrTCP.sin_addr.S_un.S_un_b.s_b4 << ")" << std::endl;
        //std::cout << "�|�[�g�ԍ�:" << (int)connectMemberList[i].addrTCP.sin_port << std::endl;
    }
}

// �R���\�[����ʂɎw���IP�A�h���X��\��
void ClientServerModel::WriteConsoleIPAdress(sockaddr_in srcAddr)
{
    ConsoleFunc::Write("IPAddress : ");
    ConsoleFunc::WriteEndl((
          std::to_string((int)srcAddr.sin_addr.S_un.S_un_b.s_b1) + "."
        + std::to_string((int)srcAddr.sin_addr.S_un.S_un_b.s_b2) + "."
        + std::to_string((int)srcAddr.sin_addr.S_un.S_un_b.s_b3) + "."
        + std::to_string((int)srcAddr.sin_addr.S_un.S_un_b.s_b4)
        ).c_str());
    ConsoleFunc::WriteEndl(("�|�[�g�ԍ�:" + std::to_string((int)srcAddr.sin_port)).c_str());
}

//��������
bool ClientServerModel::Initialize()
{
    return false;
}

// �I������
void ClientServerModel::Finalize()
{
}

// �w���TCP�A�h���X����UDP�A�h���X���擾
sockaddr_in ClientServerModel::ChangeUDPAddress(sockaddr_in srcAddr)
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

    //std::cout << "�z�X�g��";
    WriteConsoleIPAdress(resultAddr);

    return resultAddr;
}
//-------------------------------------------------------------------------------------------------------------
// �N���C�A���g�T�[�o�[�^�@�z�X�g
//-------------------------------------------------------------------------------------------------------------

// �R���X�g���N�^
HostCS::HostCS()
{
}

// �f�X�g���N�^
HostCS::~HostCS()
{
    if (tcpServer != nullptr)
        delete tcpServer;
    if (udpServer != nullptr)
        delete udpServer;
}

// ��������
bool HostCS::Initialize()
{
    ClientServerModel::Initialize();

    //���ꂼ��̃T�[�o�[���N��
    tcpServer = new CommunicateTCPServer();
    udpServer = new CommunicateUDPServer();
    multicastSender = new CommunicateMultiCastServer();
    multicastReciever = new CommunicateMultiCastClient();


    ownListIndex = 0;

    //���ꂼ��̃T�[�o�[���N��
    if (tcpServer->Initialize(appPortIndex) == false)
        return false;

    if (udpServer->Initialize(appPortIndex) == false)
        return false;

    // �}���`�L���X�g�p�T�[�o�[�N��
    if (multicastSender->Initialize(multiPortIndex) == false)
        return false;


    // �}���`�L���X�g�p�N���C�A���g�N��
    if (multicastReciever->Initialize(multiPortIndex) == false)
        return false;

    // �z�X�g���g�̏���ۑ�
    connectMemberList[0].socketTCP = tcpServer->GetSocket();
    connectMemberList[0].addrTCP = tcpServer->GetSocketAddress();
    connectMemberList[0].addrUDP = ChangeUDPAddress(tcpServer->GetSocketAddress());
    return true;

}

// �I������
void HostCS::Finalize()
{
    // TCP�̐ڑ���ؒf

    // UDP�̐ڑ���ؒf

    // �}���`�L���X�g�̐ڑ���ؒf


}


// �f�[�^�𑗐M
void HostCS::SendData()
{
}

// �`���b�g���b�Z�[�W�𑗐M
void HostCS::SendChatMessage(char chatMessage[128])
{
    ChatCommand chatCom;
    chatCom.actionType = static_cast<char>(ActionCommandType::Chat);
    //chatCom.actionFrame = SceneManager::Instance().GetActionFrame() + ragTime;// 1�b��(10�t���[����)�ɕ\��
    //snprintf(chatCom.message, sizeof(chatMessage), chatMessage);
    strcpy_s(chatCom.message, strlen(chatMessage)+1, chatMessage);

    for (int i = 1; i < MaxMemberSize; i++)
    {
        if (connectMemberList[i].socketTCP != INVALID_SOCKET)
            tcpServer->Send(connectMemberList[i].socketTCP, (char*)&chatCom, sizeof(ChatCommand));
    }
}

// �ڑ��҂�
bool HostCS::WaitingConnect()
{
    int connectIndex = GetEmptyIndex();

    if (multicastReciever->Recieve())
    {
        //�Q�����������Ƃ�����TCP��UDP�̏���������
        buffer = multicastReciever->GetBuffer();
        action = buffer[0];

        //�Q�����̏ꍇ
        if (action == static_cast<int>(ActionCommandType::Join))
        {
            ConsoleFunc::Write("�Q�X�g��������܂���");

            // �ʐM���̐l�����ő吔�𒴂����ꍇ�͎Q�����ۂ��o��
            if (connectIndex < 0)
            {
                // �Q�����Ă������̂ɑ΂��đޏo���o��
                ExitCommand exitCom;
                exitCom.actionType = (char)ActionCommandType::Exit;
                exitCom.index = -1;
                multicastSender->Send((char*)&exitCom, sizeof(ExitCommand));
            }
            else //�l�����ő吔�𒴂��Ă��Ȃ��ꍇ
            {
                // �Q�����Ă������̂ɑ΂��ĎQ�������o��
                JoinPermitCommand joinPerCom;
                joinPerCom.actionType = (char)ActionCommandType::JoinPermit;
                joinPerCom.index = connectIndex;
                for (int i = 0; i < MaxMemberSize; i++)
                {
                    //�\�P�b�g���������Ă���ꍇ�̂݃����o�[�̃��X�g���R�s�[
                    if (connectMemberList[i].socketTCP == INVALID_SOCKET)continue;
                    
                    joinPerCom.memberList[i] = connectMemberList[i];
                }
                multicastSender->Send((char*)&joinPerCom, sizeof(JoinPermitCommand));
            }
         
        }
    }

    ComputerData data{};

    // TCP�ڑ��҂�
    if (tcpServer->AcceptOnSelect(appPortIndex, &data))
    {
        // �Q�X�g�̃f�[�^��ۊ�
        RegisterList(connectIndex, data);
        tcpServer->SetList(&connectMemberList[connectIndex].socketTCP);
        char mess[] = "nice to meet you.";
        SendChatMessage(mess);
    }

    return false;
}

// �f�[�^����M
void HostCS::RecieveData()
{
    // �}���`�L���X�g�ʐM�Ŏ�M
    if (multicastReciever->Recieve())
    {
        // ��M�����f�[�^����R�}���h���擾
        ConversionCommandFromData(udpServer->GetBuffer());
    }


    buffer = nullptr;
    action = -1;

    // UDP�ʐM�Ŏ�M
    if (udpServer->Recieve().sin_addr.S_un.S_addr != 0)// ��M�ɐ��������ꍇ
    {
        // ��M�����f�[�^����R�}���h���擾
        ConversionCommandFromData(udpServer->GetBuffer());

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
    for (int i = 0; i < MaxMemberSize; i++)
    {
        //�ʐM���̑���̏�񂪂���Ƃ�������M
        if (connectMemberList[i].socketTCP == INVALID_SOCKET)continue;
        if (tcpServer->Recieve(connectMemberList[i].socketTCP))
        {
            // ��M�����f�[�^����R�}���h���擾
            ConversionCommandFromData(tcpServer->GetBuffer());
        }
    }

}

// �Q�X�g�̐ڑ��҂�
void HostCS::Accept()
{
}

// �Ȃ��Ă����N���C�A���g�̏���ۑ�
void HostCS::RegisterList(int index, ComputerData data)
{
    // ���ɒʐM�ς��ǂ����m�F����
    int connectIndex = CheckAlreadyConnect(&data.addrTCP);

    if (connectIndex >= 0)// ���ɒʐM�ς̑���ł���Γo�^���Ȃ�
    {
        ConsoleFunc::WriteEndl("���ɓ��o�ςł�");
        return;
    }

    // �V�K�̑���̏ꍇ
    // �V�K�o�^������
    connectMemberList[index] = data;

    ConsoleFunc::WriteEndl(("�V�K�N���C�A���g��o�^���܂���" + std::to_string(index)).c_str());

    // �Q�����R�}���h�𑗐M
    //JoinPermitCommand joinPermitCom;
    //joinPermitCom.actionType = (char)ActionCommandType::JoinPermit;
    //joinPermitCom.index = index;
    //tcpServer->Send(connectMemberList[index].socketTCP, (char*)&joinPermitCom, sizeof(JoinPermitCommand));

    // TCP�̃A�h���X����UDP�̃A�h���X�֕ϊ�
    connectMemberList[index].addrUDP = ChangeUDPAddress(connectMemberList[index].addrTCP);

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
int HostCS::CheckAlreadyConnect(sockaddr_in* addr)
{
    for (int i = 0; i < MaxMemberSize; i++)
    {
        // �A�h���X�ƃ|�[�g�ԍ�����v���邩���ׂ�
        if (addr->sin_addr.S_un.S_addr == connectMemberList[i].addrUDP.sin_addr.S_un.S_addr &&
            addr->sin_port == connectMemberList[i].addrUDP.sin_port)
            return i;
    }
    return -1;
}

// �����Ă����R�}���h����������
void HostCS::ConversionCommandFromData(char* buff)
{
    buffer = buff;
    action = buffer[0];
    // �����Ă����R�}���h�ɑΉ����鏈��������
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
        {
            ConsoleFunc::WriteEndl("�`���b�g��M");
            ChatCommand* chatCom = (ChatCommand*)(buff);
            ConsoleFunc::Write(chatCom->message);
            //ConsoleFunc::WriteEndl(std::to_string(chatCom->actionFrame).c_str());
            //ChatData* chatData = new ChatData(chatCom->message, chatCom->actionFrame);
            //recieveMessages.emplace_back(chatData);
        }
        break;
    default:
        ConsoleFunc::Write("��M�����R�}���h�F");
        //ConsoleFunc::WriteEndl(magic_enum::enum_name(static_cast<ActionCommandType>(action)).data());
        break;
    }
    std::cout << std::endl;
}



//-------------------------------------------------------------------------------------------------------------
// �N���C�A���g�T�[�o�[�^�@�Q�X�g
//-------------------------------------------------------------------------------------------------------------

// �R���X�g���N�^
GuestCS::GuestCS()
{
}

// �f�X�g���N�^
GuestCS::~GuestCS()
{
    if (tcpClient != nullptr)
        delete tcpClient;
    if (udpClient != nullptr)
        delete udpClient;
}

// ��������
bool GuestCS::Initialize()
{
    //���ꂼ��̃N���C�A���g���N��
    tcpClient = new CommunicateTCPClient();
    udpClient = new CommunicateUDPClient();
    multicastSender = new CommunicateMultiCastServer();
    multicastReciever = new CommunicateMultiCastClient();

    // �N���C�A���g���N��

    // TCP�T�[�o�[�֐ڑ�

    // UDP�T�[�o�[�֐ڑ�

    // �}���`�L���X�g�ɓo�^

    ClientServerModel::Initialize();

    ownListIndex = 1;

    // �}���`�L���X�g�p�T�[�o�[�N��
    if (multicastSender->Initialize(multiPortIndex) == false)
        return false;

    // �}���`�L���X�g�p�N���C�A���g�N��
    if (!multicastReciever->Initialize(multiPortIndex))
        return false;

    //�}���`�L���X�g�ւ̓o�^������������Q���R�}���h���z�X�g�֑��M����
    // �Q���R�}���h�𑗐M
    int roomindex = 2;
    int password = 111;
    JoinCommand joinCom;
    joinCom.actionType = (char)ActionCommandType::Join;
    joinCom.roomIndex = roomindex;
    joinCom.password = password;
    multicastSender->Send((char*)&joinCom, sizeof(JoinCommand));


    // TCP�N���C�A���g���N��
    //if (tcpClient->Initialize(appPortIndex, hostAddress) == false)
    //    return false;

    // ���o������M����
    //if (tcpClient->Recieve())
    //    ConversionCommandFromData(tcpClient->GetBuffer());

    // UDP�T�[�o�[�̋N��
    //if (udpClient->Initialize(appPortIndex + ownListIndex, hostAddress) == false)
    //    return false;


    // ���O�̏����󂯎��
    //if (tcpClient->Recieve())
    //    ConversionCommandFromData(tcpClient->GetBuffer());

    return true;

}

//�I������
void GuestCS::Finalize()
{
    // TCP�T�[�o�[�ւ̐ڑ���ؒf
    
    // UDP�T�[�o�[�ւ̐ڑ���ؒf
    
    // �}���`�L���X�g�T�[�o�[�ւ̐ڑ���ؒf

}

// �f�[�^�𑗐M
void GuestCS::SendData()
{
}

// �f�[�^����M
void GuestCS::RecieveData()
{
    // �f�[�^����M
    buffer = nullptr;
    action = -1;
    if (udpClient->Recieve())
    { 
        // ��M�����f�[�^����R�}���h�֕ϊ�
        ConversionCommandFromData(udpClient->GetBuffer());
    }
    if (multicastReciever->Recieve())
    {
        // ��M�����f�[�^����R�}���h�֕ϊ�
        ConversionCommandFromData(multicastReciever->GetBuffer());
    }
    if (tcpClient->RecieveOnSelect())
    {
        // ��M�����f�[�^����R�}���h�֕ϊ�
        ConversionCommandFromData(tcpClient->GetBuffer());
    }
}

// �����Ă����R�}���h����������
void GuestCS::ConversionCommandFromData(char* buff)
{
    buffer = buff;
    action = buff[0];

    switch (action)
    {
    case static_cast<int>(ActionCommandType::JoinPermit):// ��������
        ConsoleFunc::WriteEndl("���o������M");
        {
            JoinPermitCommand* joinPermitCom = (JoinPermitCommand*)(buff);
            ownListIndex = (int)joinPermitCom->index;
            //SceneManager::Instance().SetHostStartActionFrame(joinPermitCom->actionFrame);
            std::cout << "�����̎��ʔԍ�:" << ownListIndex << std::endl;
            for (int i = 0; i < MaxMemberSize; i++)
            {
                if (joinPermitCom->memberList[i].addrTCP.sin_addr.S_un.S_addr == 0)continue;
                connectMemberList[i] = joinPermitCom->memberList[i];
                std::cout << "�Ȃ����Ă���PC��IPAddress(";
                std::cout << (int)connectMemberList[i].addrTCP.sin_addr.S_un.S_un_b.s_b1 << ".";
                std::cout << (int)connectMemberList[i].addrTCP.sin_addr.S_un.S_un_b.s_b2 << ".";
                std::cout << (int)connectMemberList[i].addrTCP.sin_addr.S_un.S_un_b.s_b3 << ".";
                std::cout << (int)connectMemberList[i].addrTCP.sin_addr.S_un.S_un_b.s_b4 << ")" << std::endl;
                std::cout << "�|�[�g�ԍ�:" << (int)connectMemberList[i].addrTCP.sin_port << std::endl;
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
        {
            ConsoleFunc::WriteEndl("�`���b�g��M");
            ChatCommand* chatCom = (ChatCommand*)(tcpClient->GetBuffer());
            ConsoleFunc::WriteEndl(chatCom->message);
            //ConsoleFunc::WriteEndl(std::to_string(chatCom->actionFrame).c_str());
            //ChatData* chatData = new ChatData(chatCom->message, chatCom->actionFrame);
            //recieveMessages.emplace_back(chatData);
        }
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

bool GuestCS::WaitingConnect()
{
    //�Q�����������Ƃ�����TCP��UDP�̏���������
    if (multicastReciever->Recieve())
    {
        buffer = multicastReciever->GetBuffer();
        action = buffer[0];

        //�Q�����̏ꍇ
        if (action == static_cast<int>(ActionCommandType::JoinPermit))
        {
            ConsoleFunc::WriteEndl("�z�X�g��������܂���");

            //���M�҂�IP�A�h���X���擾
            sockaddr_in addr = multicastReciever->GetFromAddress();
            std::string addrStr=
                std::to_string(addr.sin_addr.S_un.S_un_b.s_b1) + "." +
                std::to_string(addr.sin_addr.S_un.S_un_b.s_b2) + "." +
                std::to_string(addr.sin_addr.S_un.S_un_b.s_b3) + "." +
                std::to_string(addr.sin_addr.S_un.S_un_b.s_b4);
            strcpy_s(hostAddress, strlen(addrStr.c_str()) + 1, addrStr.c_str());

            ConsoleFunc::Write("�z�X�g��");
            WriteConsoleIPAdress(addr);

            // TCP�֐ڑ�
            // TCP�N���C�A���g���N��
            if (tcpClient->Initialize(appPortIndex, hostAddress) == false)
                return false;

            // ���o������M����
            //if (tcpClient->Recieve())
            //    ConversionCommandFromData(tcpClient->GetBuffer());
            std::cout << "�����̎��ʔԍ�:" << ownListIndex << std::endl;
            for (int i = 0; i < MaxMemberSize; i++)
            {
                //if (joinPermitCom->preJoinData[i].addrTCP.sin_addr.S_un.S_addr == 0)continue;
                //otherGuestData[i] = joinPermitCom->preJoinData[i];
                //std::cout << "�Ȃ����Ă���PC��IPAddress(";
                //std::cout << (int)connectMemberList[i].addrTCP.sin_addr.S_un.S_un_b.s_b1 << ".";
                //std::cout << (int)connectMemberList[i].addrTCP.sin_addr.S_un.S_un_b.s_b2 << ".";
                //std::cout << (int)connectMemberList[i].addrTCP.sin_addr.S_un.S_un_b.s_b3 << ".";
                //std::cout << (int)connectMemberList[i].addrTCP.sin_addr.S_un.S_un_b.s_b4 << ")" << std::endl;
                //std::cout << "�|�[�g�ԍ�:" << (int)connectMemberList[i].addrTCP.sin_port << std::endl;
                //if (connectMemberList[i].addrTCP.sin_addr.S_un.S_addr == 0)continue;
                // �v���C���[�̎Q�������Q�[���V�[���ŏ���
                //if ((int)joinPermitCom->ownIndex != i)
                //    SceneManager::Instance().JoinPlayer(i, TypeConversionShort3::ChangeFloat3Decimal2(joinPermitCom->joinPosition[i]));
            }

            // UDP�T�[�o�[�̋N��
            if (udpClient->Initialize(appPortIndex + ownListIndex, hostAddress) == false)
                return false;
        }
        ConversionCommandFromData(multicastReciever->GetBuffer());
    }
    return false;
}

//���b�Z�[�W�𑗐M
void GuestCS::SendChatMessage(char chatMessage[128])
{
    ChatCommand chatCom;
    chatCom.actionType = static_cast<char>(ActionCommandType::Chat);
    //chatCom.actionFrame = SceneManager::Instance().GetActionFrame() + ragTime;// 1�b��(10�t���[����)�ɕ\��
    //snprintf(chatCom.message, sizeof(chatMessage), chatMessage);
    strcpy_s(chatCom.message, strlen(chatMessage) + 1, chatMessage);
  
    tcpClient->Send((char*)&chatCom, sizeof(ChatCommand));

    for (int i = 0; i < MaxMemberSize; i++)
    {
        if (connectMemberList[i].addrUDP.sin_addr.S_un.S_addr == 0)continue;
        udpClient->Send(connectMemberList[i].addrUDP, (char*)&chatCom, sizeof(ChatCommand));
    }
}

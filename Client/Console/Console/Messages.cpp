#define MAXNAMELEN  32

enum MsgType {
    Join,// Client joining game at server  
    Leave,// Client leaving game  
    Change,// Information to clients  
    Event,// Information from clients to server  
    TextMessage// Send text messages to one or all
};// Included first in all messagesstruct

struct MsgHead {
    unsigned int length;// Total length for whole message  
    unsigned int seq_no;  // Sequence number  
    unsigned int id;// Client ID or 0;
    MsgType   type;// Type of message
};

enum ObjectDesc {
    Human,
    NonHuman,
    Vehicle,
    StaticObject
};

enum ObjectForm {
    Cube,
    Sphere,
    Pyramid,
    Cone
};

struct JoinMsg {
    MsgHead head;
    ObjectDesc  desc;
    ObjectForm  form;
    char name[MAXNAMELEN];
};

struct LeaveMsg {
    MsgHead  head;
};

enum ChangeType {
    NewPlayer,
    PlayerLeave,
    NewPlayerPosition
};
// Included first in all Change messages
struct ChangeMsg {
    MsgHead head;
    ChangeType type;
};

struct Coordinate {
    int x;
    int y;
};

struct NewPlayerPositionMsg {
    ChangeMsg msg; //Change message header
    Coordinate pos; //New object position
    Coordinate dir; //New object direction
};

// Messages of type Event (Client -> Server)
enum EventType
{
    Move
};
// Included first in all Event messages
struct EventMsg
{
    MsgHead head;
    EventType type;
};
// Variantions of EventMsg
struct MoveEvent
{
    EventMsg event;
    Coordinate pos; //New object position
    Coordinate dir; //New object direction
};

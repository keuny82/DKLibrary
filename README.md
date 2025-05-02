Boost 1.88 설치 필요
설치 위치 : C:\boost



<div class="mermaid">
classDiagram

class TCPSession
	TCPSession: asio_io_context m_IOContext
	TCPSession: asio_ip_tcp_socket m_Socket
	TCPSession: asio_io_context_strand m_Strand
	TCPSession: asio_io_tcp_resolver m_Resolver
	TCPSession: TCPEventQueue& m_EventQueue
	TCPSession: mutex m_Lock
	TCPSession: SendQueue m_SendQueue
	TCPSession: RecvQueue m_RecvQueue
	TCPSession: EncoderPtr m_Encoder
	TCPSession: EndPoint m_EndPoint
	TCPSession: bool m_Inbound
	TCPSession: bool m_Connecting
	TCPSession: bool m_Connected
	TCPSession: bool m_LazyClose
	TCPSession: TCPSessionNo m_No
	TCPSession: bool m_Persistent
	TCPSession: time_t m_LastReceivedTime
	TCPSession: close()
	TCPSession: connect()
	TCPSession: resonnect()
	TCPSession: send()
	TCPSession: onAccepted()
	TCPSession: onResolved()
	TCPSession: onConnected()
	TCPSession: onClosed()
	TCPSession: onSent()
	TCPSession: onReceived()
end

class RecvQueue
	RecvQueue: StreamQueue m_Queue
	RecvQueue: perpare()
	RecvQueue: buffers()
	RecvQueue: getReadableSize()
	RecvQueue: getWriteableSize()
	RecvQueue: commit()
	RecvQueue: peek()
	RecvQueue: pop()
	RecvQueue: detachable()
	RecvQueue: detacj()
end

class SendQueue
	SendQueue: StreamQueue m_Queue	
	SendQueue: push()
	SendQueue: consume()
	SendQueue: buffers()
	SendQueue: size()
	SendQueue: empty()
end

class TCPAcceptor
	TCPAcceptor: asio_io_context m_IOContext
	TCPAcceptor: TCPEventListenerPtr m_EventListener
	TCPAcceptor: asio_io_tcp_acceptor m_Acceptor
	TCPAcceptor: start()
	TCPAcceptor: createSession()
	TCPAcceptor: asyncAccept()
	TCPAcceptor: onAccepted()
end

class TCPMessageHandler
	TCPMessageHandler: Callback m_Callback
	TCPMessageHandler: handle()
	TCPMessageHandler: getMessageNo()
end

class TCPEventListener
	TCPEventListener: ClosedHandler m_ClosedHandler
	TCPEventListener: AcceptedHandler m_AcceptedHandler
	TCPEventListener: ConnectedHandler	m_ConnectedHandler
	TCPEventListener: MessageHandlerMap	m_MessageHandlers
	TCPEventListener: dispach()
	TCPEventListener: isAsync()
	TCPEventListener: context()
	TCPEventListener: handleMessage()
	TCPEventListener: addMessageCallback()
end


class TCPAsyncEventListener
	TCPAsyncEventListener: TCPEventContextList m_EventContexts
	TCPAsyncEventListener: thread m_Dispatchers
	TCPAsyncEventListener: bool m_Stop
	TCPAsyncEventListener: time_t m_DisPatchIntervalMS
	TCPAsyncEventListener: size_t m_Pos
	TCPAsyncEventListener: dispatch()
	TCPAsyncEventListener: isAsync()
	TCPAsyncEventListener: context()
end

class TCPSyncEventListener
	TCPSyncEventListener: TCPEventContext m_EventContext
	TCPSyncEventListener: dispatch()
	TCPSyncEventListener: isAsync()
	TCPSyncEventListener: context()
end

class Encoder
	Encoder: TCPSession* m_Session
	Encoder: encode()
	Encoder: decode()
	Encoder: getHeaderSize()
end

class TCPService
	TCPService: asio_io_context m_IOContext
	TCPService: thread m_Workers
	TCPService: asio_executor_work_guard m_Work
	TCPService: AcceptorList m_Acceptors
	TCPService: EventListenerSet m_AsyncEventListeners
	TCPService: EventListenerSet m_SyncEventListeners
	TCPService: open()
	TCPService: close()
	TCPService: run()
	TCPService: startServer()
	TCPService: connectServer()
	TCPService: addEventListener()
end


SendQueue --> TCPSession
RecvQueue --> TCPSession
TCPEventListener <|-- TCPAsyncEventListener
TCPEventListener <|-- TCPSyncEventListener
TCPAsyncEventListener --> TCPService
TCPSyncEventListener --> TCPService
TCPAcceptor --> TCPService
TCPSession --> TCPService
TCPMessageHandler --> TCPEventListener

</div>

<script src="https://cdn.jsdelivr.net/npm/mermaid/dist/mermaid.min.js"></script>
<script>mermaid.initialize({startOnLoad:true});</script>

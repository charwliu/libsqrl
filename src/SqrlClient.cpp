#include "sqrl_internal.h"

#include "SqrlClient.h"
#include "SqrlAction.h"
#include "SqrlUser.h"

#define SQRL_CALLBACK_SAVE_SUGGESTED 0
#define SQRL_CALLBACK_SELECT_USER 1
#define SQRL_CALLBACK_SELECT_ALT 2
#define SQRL_CALLBACK_ACTION_COMPLETE 3
#define SQRL_CALLBACK_AUTH_REQUIRED 4
#define SQRL_CALLBACK_SEND 5
#define SQRL_CALLBACK_ASK 6
#define SQRL_CALLBACK_PROGRESS 7

SqrlClient *SqrlClient::client = NULL;

SqrlClient::SqrlClient() {
	if( SqrlClient::client != NULL ) {
		// Enforce a single SqrlClient object
		exit( 1 );
	}
	SqrlClient::client = this;
	this->myThread = new std::thread( SqrlClient::clientThread );
}

SqrlClient::~SqrlClient() {
	this->stopping = true;
	this->myThread->join();
	SqrlClient::client = NULL;
	delete this->myThread;
}

SqrlClient *SqrlClient::getClient() {
	return SqrlClient::client;
}

void SqrlClient::onLoop() {
}

void SqrlClient::loop() {
	this->onLoop();
	SqrlAction *action;
	while( !this->callbackQueue.empty() ) {
		struct CallbackInfo *info = this->callbackQueue.front();
		switch( info->cbType ) {
		case SQRL_CALLBACK_SAVE_SUGGESTED:
			this->onSaveSuggested( (SqrlUser*)info->ptr );
			((SqrlUser*)info->ptr)->release();
			break;
		case SQRL_CALLBACK_SELECT_USER:
			action = (SqrlAction*)info->ptr;
			this->onSelectUser( action );
			break;
		case SQRL_CALLBACK_SELECT_ALT:
			action = (SqrlAction*)info->ptr;
			this->onSelectAlternateIdentity( action );
			break;
		case SQRL_CALLBACK_ACTION_COMPLETE:
			action = (SqrlAction*)info->ptr;
			this->onActionComplete( action );
			break;
		case SQRL_CALLBACK_AUTH_REQUIRED:
			action = (SqrlAction*)info->ptr;
			this->onAuthenticationRequired( action, info->credentialType );
			break;
		case SQRL_CALLBACK_SEND:
			action = (SqrlAction*)info->ptr;
			this->onSend( action, *info->str[0], *info->str[1] );
			break;
		case SQRL_CALLBACK_ASK:
			action = (SqrlAction*)info->ptr;
			this->onAsk( action, *info->str[0], *info->str[1], *info->str[2] );
			break;
		case SQRL_CALLBACK_PROGRESS:
			action = (SqrlAction*)info->ptr;
			this->onProgress( action, info->progress );
			break;
		}
		this->callbackQueue.pop();
		delete info;
	}
	if( this->actions.size() > 0 ) {
		action = this->actions.front();
		if( action ) {
			if( action->exec() ) {
				this->actionMutex.lock();
				this->actions.pop_front();
				this->actions.push_back( action );
				this->actionMutex.unlock();
			}
		}
	}
}

void SqrlClient::callSaveSuggested( SqrlUser * user ) {
	user->hold();
	struct CallbackInfo *info = new struct CallbackInfo();
	info->cbType = SQRL_CALLBACK_SAVE_SUGGESTED;
	info->ptr = user;
	this->callbackQueue.push( info );
}

void SqrlClient::callSelectUser( SqrlAction * transaction ) {
	struct CallbackInfo *info = new struct CallbackInfo();
	info->cbType = SQRL_CALLBACK_SELECT_USER;
	info->ptr = transaction;
	this->callbackQueue.push( info );
}

void SqrlClient::callSelectAlternateIdentity( SqrlAction * transaction ) {
	struct CallbackInfo *info = new struct CallbackInfo();
	info->cbType = SQRL_CALLBACK_SELECT_ALT;
	info->ptr = transaction;
	this->callbackQueue.push( info );
}

void SqrlClient::callActionComplete( SqrlAction * transaction ) {
	struct CallbackInfo *info = new struct CallbackInfo();
	info->cbType = SQRL_CALLBACK_ACTION_COMPLETE;
	info->ptr = transaction;
	this->callbackQueue.push( info );
}

void SqrlClient::callProgress( SqrlAction * transaction, int progress ) {
	struct CallbackInfo *info = new struct CallbackInfo();
	info->cbType = SQRL_CALLBACK_PROGRESS;
	info->ptr = transaction;
	info->progress = progress;
	this->callbackQueue.push( info );
}

void SqrlClient::callAuthenticationRequired( SqrlAction * transaction, Sqrl_Credential_Type credentialType ) {
	struct CallbackInfo *info = new struct CallbackInfo();
	info->cbType = SQRL_CALLBACK_AUTH_REQUIRED;
	info->ptr = transaction;
	info->credentialType = credentialType;
	this->callbackQueue.push( info );
}

void SqrlClient::callSend( SqrlAction * t, std::string *url, std::string * payload ) {
	struct CallbackInfo *info = new struct CallbackInfo();
	info->cbType = SQRL_CALLBACK_SEND;
	info->ptr = t;
	info->str[0] = new std::string( *url );
	info->str[1] = new std::string( *payload );
	this->callbackQueue.push( info );
}

void SqrlClient::callAsk( SqrlAction * transaction, std::string * message, std::string * firstButton, std::string * secondButton ) {
	struct CallbackInfo *info = new struct CallbackInfo();
	info->cbType = SQRL_CALLBACK_ASK;
	info->ptr = transaction;
	info->str[0] = new std::string( *message );
	info->str[1] = new std::string( *firstButton );
	info->str[2] = new std::string( *secondButton );
	this->callbackQueue.push( info );
}

void SqrlClient::clientThread() {
	SqrlClient *client;
	while( (client = SqrlClient::getClient()) && ! client->stopping ) {
		client->loop();
		sqrl_sleep( 100 );
	}
}

SqrlClient::CallbackInfo::CallbackInfo() {
	this->cbType = 0;
	this->progress = 0;
	this->credentialType = SQRL_CREDENTIAL_PASSWORD;
	this->ptr = NULL;
	this->str[0] = NULL;
	this->str[1] = NULL;
	this->str[2] = NULL;

}

SqrlClient::CallbackInfo::~CallbackInfo() {
	int i;
	if( this->ptr ) {
		if( this->cbType == SQRL_CALLBACK_SAVE_SUGGESTED ) {
			SqrlUser *user = (SqrlUser*)this->ptr;
			user->release();
		}
	}
	for( i = 0; i < 3; i++ ) {
		if( this->str[i] ) {
			delete this->str[i];
		}
	}
}

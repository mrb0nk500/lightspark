/**************************************************************************
    Lightspark, a free flash player implementation

    Copyright (C) 2009-2013  Alessandro Pignotti (a.pignotti@sssup.it)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/

#include "scripting/avmplus/avmplus.h"
#include "scripting/toplevel/Number.h"
#include "scripting/toplevel/Integer.h"
#include "scripting/flash/utils/ByteArray.h"
#include "scripting/flash/system/flashsystem.h"
#include "scripting/class.h"
#include "scripting/argconv.h"
#include "threading.h"
#include "abc.h"
#include "platforms/engineutils.h"

using namespace lightspark;

avmplusFile::avmplusFile(ASWorker* wrk, Class_base* c):
	ASObject(wrk,c)
{
}

void avmplusFile::sinit(Class_base* c)
{
	CLASS_SETUP_NO_CONSTRUCTOR(c, ASObject, CLASS_SEALED);
	c->setDeclaredMethodByQName("exists","",Class<IFunction>::getFunction(c->getSystemState(),exists),NORMAL_METHOD,false);
	c->setDeclaredMethodByQName("read","",Class<IFunction>::getFunction(c->getSystemState(),read),NORMAL_METHOD,false);
	c->setDeclaredMethodByQName("write","",Class<IFunction>::getFunction(c->getSystemState(),write),NORMAL_METHOD,false);
	c->setDeclaredMethodByQName("readByteArray","",Class<IFunction>::getFunction(c->getSystemState(),readByteArray),NORMAL_METHOD,false);
	c->setDeclaredMethodByQName("writeByteArray","",Class<IFunction>::getFunction(c->getSystemState(),writeByteArray),NORMAL_METHOD,false);
}

ASFUNCTIONBODY_ATOM(avmplusFile,exists)
{
	tiny_string filename;
	ARG_UNPACK_ATOM(filename);
	asAtomHandler::setBool(ret,wrk->getSystemState()->getEngineData()->FileExists(wrk->getSystemState(),filename,false));
}
ASFUNCTIONBODY_ATOM(avmplusFile,read)
{
	tiny_string filename;
	ARG_UNPACK_ATOM(filename);
	if (!wrk->getSystemState()->getEngineData()->FileExists(wrk->getSystemState(),filename,false))
		throwError<ASError>(kFileOpenError,filename);
	ret = asAtomHandler::fromObject(abstract_s(wrk,wrk->getSystemState()->getEngineData()->FileRead(wrk->getSystemState(),filename,false)));
}
ASFUNCTIONBODY_ATOM(avmplusFile,write)
{
	tiny_string filename;
	tiny_string data;
	ARG_UNPACK_ATOM(filename)(data);
	wrk->getSystemState()->getEngineData()->FileWrite(wrk->getSystemState(),filename,data,false);
}
ASFUNCTIONBODY_ATOM(avmplusFile,readByteArray)
{
	tiny_string filename;
	ARG_UNPACK_ATOM(filename);
	if (!wrk->getSystemState()->getEngineData()->FileExists(wrk->getSystemState(),filename,false))
		throwError<ASError>(kFileOpenError,filename);
	ByteArray* res = Class<ByteArray>::getInstanceS(wrk);
	wrk->getSystemState()->getEngineData()->FileReadByteArray(wrk->getSystemState(),filename,res,0,UINT32_MAX,false);
	ret = asAtomHandler::fromObject(res);
}
ASFUNCTIONBODY_ATOM(avmplusFile,writeByteArray)
{
	tiny_string filename;
	_NR<ByteArray> data;
	ARG_UNPACK_ATOM(filename)(data);
	wrk->getSystemState()->getEngineData()->FileWriteByteArray(wrk->getSystemState(),filename,data.getPtr(),0,UINT32_MAX,false);
}

avmplusSystem::avmplusSystem(ASWorker* wrk, Class_base* c):
	ASObject(wrk,c)
{
}

void avmplusSystem::sinit(Class_base* c)
{
	CLASS_SETUP_NO_CONSTRUCTOR(c, ASObject, CLASS_SEALED);
	c->setDeclaredMethodByQName("getFeatures","",Class<IFunction>::getFunction(c->getSystemState(),getFeatures),NORMAL_METHOD,false);
	c->setDeclaredMethodByQName("queueCollection","",Class<IFunction>::getFunction(c->getSystemState(),queueCollection),NORMAL_METHOD,false);
	c->setDeclaredMethodByQName("forceFullCollection","",Class<IFunction>::getFunction(c->getSystemState(),forceFullCollection),NORMAL_METHOD,false);
	c->setDeclaredMethodByQName("getAvmplusVersion","",Class<IFunction>::getFunction(c->getSystemState(),getAvmplusVersion),NORMAL_METHOD,false);
	c->setDeclaredMethodByQName("pauseForGCIfCollectionImminent","",Class<IFunction>::getFunction(c->getSystemState(),pauseForGCIfCollectionImminent),NORMAL_METHOD,false);
	c->setDeclaredMethodByQName("getRunmode","",Class<IFunction>::getFunction(c->getSystemState(),getRunmode),NORMAL_METHOD,false);
	c->setDeclaredMethodByQName("isDebugger","",Class<IFunction>::getFunction(c->getSystemState(),isDebugger),NORMAL_METHOD,false);
	c->setDeclaredMethodByQName("isGlobal","",Class<IFunction>::getFunction(c->getSystemState(),isGlobal),NORMAL_METHOD,false);
	c->setDeclaredMethodByQName("freeMemory","",Class<IFunction>::getFunction(c->getSystemState(),_freeMemory),GETTER_METHOD,false);
	c->setDeclaredMethodByQName("totalMemory","",Class<IFunction>::getFunction(c->getSystemState(),_totalMemory),GETTER_METHOD,false);
	c->setDeclaredMethodByQName("privateMemory","",Class<IFunction>::getFunction(c->getSystemState(),_privateMemory),GETTER_METHOD,false);
	c->setDeclaredMethodByQName("swfVersion","",Class<IFunction>::getFunction(c->getSystemState(),_swfVersion),GETTER_METHOD,false);
	c->setDeclaredMethodByQName("argv","",Class<IFunction>::getFunction(c->getSystemState(),argv),GETTER_METHOD,false);
	c->setDeclaredMethodByQName("exec","",Class<IFunction>::getFunction(c->getSystemState(),exec),NORMAL_METHOD,false);
	c->setDeclaredMethodByQName("write","",Class<IFunction>::getFunction(c->getSystemState(),write),NORMAL_METHOD,false);
	c->setDeclaredMethodByQName("sleep","",Class<IFunction>::getFunction(c->getSystemState(),sleep),NORMAL_METHOD,false);
	c->setDeclaredMethodByQName("exit","",Class<IFunction>::getFunction(c->getSystemState(),exit),NORMAL_METHOD,false);
	c->setDeclaredMethodByQName("trace","",Class<IFunction>::getFunction(c->getSystemState(),lightspark::trace),NORMAL_METHOD,false);
	c->setDeclaredMethodByQName("canonicalizeNumber","",Class<IFunction>::getFunction(c->getSystemState(),canonicalizeNumber),NORMAL_METHOD,false);
}

ASFUNCTIONBODY_ATOM(avmplusSystem,getFeatures)
{
	LOG(LOG_NOT_IMPLEMENTED, "avmplus.System.getFeatures is unimplemented.");
	ret = asAtomHandler::fromStringID(BUILTIN_STRINGS::EMPTY);
}
ASFUNCTIONBODY_ATOM(avmplusSystem,getRunmode)
{
	ret = asAtomHandler::fromString(wrk->getSystemState(),wrk->getSystemState()->useJit ? "jit":"interpreted");
}

ASFUNCTIONBODY_ATOM(avmplusSystem,queueCollection)
{
	LOG(LOG_NOT_IMPLEMENTED, "avmplus.System.queueCollection is unimplemented.");
	asAtomHandler::setUndefined(ret);
}

ASFUNCTIONBODY_ATOM(avmplusSystem,forceFullCollection)
{
	LOG(LOG_NOT_IMPLEMENTED, "avmplus.System.forceFullCollection is unimplemented.");
}

ASFUNCTIONBODY_ATOM(avmplusSystem,getAvmplusVersion)
{
	LOG(LOG_NOT_IMPLEMENTED, "avmplus.System.getAvmplusVersion is unimplemented.");
	ret = asAtomHandler::fromString(wrk->getSystemState(),"0");
}
ASFUNCTIONBODY_ATOM(avmplusSystem,pauseForGCIfCollectionImminent)
{
	LOG(LOG_NOT_IMPLEMENTED, "avmplus.System.pauseForGCIfCollectionImminent is unimplemented.");
}

ASFUNCTIONBODY_ATOM(avmplusSystem,isDebugger)
{
	LOG(LOG_NOT_IMPLEMENTED, "avmplus.System.isDebugger is unimplemented.");
	asAtomHandler::setBool(ret,false);
}
ASFUNCTIONBODY_ATOM(avmplusSystem,isGlobal)
{
	LOG(LOG_NOT_IMPLEMENTED, "avmplus.System.isGlobal is unimplemented.");
	asAtomHandler::setBool(ret,false);
}
ASFUNCTIONBODY_ATOM(avmplusSystem,_freeMemory)
{
	LOG(LOG_NOT_IMPLEMENTED, "avmplus.System.freeMemory is unimplemented.");
	asAtomHandler::setUInt(ret,wrk,1024);
}
ASFUNCTIONBODY_ATOM(avmplusSystem,_totalMemory)
{
	LOG(LOG_NOT_IMPLEMENTED, "avmplus.System.totalMemory is unimplemented.");
	asAtomHandler::setUInt(ret,wrk,1024);
}
//#include <sys/resource.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <errno.h>
//#include <unistd.h>

//int main() {
//   struct rusage r_usage;
//   int *p = 0;
//   while(1) {
//      p = (int*)malloc(sizeof(int)*1000);
//      int ret = getrusage(RUSAGE_SELF,&r_usage);
//      if(ret == 0)
//         printf("Memory usage: %ld kilobytes\n",r_usage.ru_maxrss);
//      else
//         printf("Error in getrusage. errno = %d\n", errno);
//      usleep(10);
//   }
//   return 0;
//}
ASFUNCTIONBODY_ATOM(avmplusSystem,_privateMemory)
{
	LOG(LOG_NOT_IMPLEMENTED, "avmplus.System.privateMemory is unimplemented.");
	asAtomHandler::setUInt(ret,wrk,1024);
}
ASFUNCTIONBODY_ATOM(avmplusSystem,_swfVersion)
{
	asAtomHandler::setUInt(ret,wrk,wrk->getSystemState()->getSwfVersion());
}

ASFUNCTIONBODY_ATOM(avmplusSystem,argv)
{
	LOG(LOG_NOT_IMPLEMENTED, "avmplus.System.argv is unimplemented.");
	ret = asAtomHandler::fromObject(Class<Array>::getInstanceS(wrk));
}
ASFUNCTIONBODY_ATOM(avmplusSystem,exec)
{
	LOG(LOG_NOT_IMPLEMENTED, "avmplus.System.exec is unimplemented.");
	if (argslen == 0)
		throwError<ArgumentError>(kWrongArgumentCountError,"exec",">0",Integer::toString(argslen));
}
ASFUNCTIONBODY_ATOM(avmplusSystem,write)
{
	LOG(LOG_NOT_IMPLEMENTED, "avmplus.System.write is unimplemented.");
}
ASFUNCTIONBODY_ATOM(avmplusSystem,sleep)
{
	uint32_t ms;
	ARG_UNPACK_ATOM(ms);
	compat_msleep(ms);
}
ASFUNCTIONBODY_ATOM(avmplusSystem,exit)
{
	LOG(LOG_NOT_IMPLEMENTED, "avmplus.System.exit is unimplemented.");
	ret = asAtomHandler::undefinedAtom;
}
ASFUNCTIONBODY_ATOM(avmplusSystem,canonicalizeNumber)
{
	_NR<ASObject> o;
	ARG_UNPACK_ATOM(o);
	switch(o->getObjectType())
	{
		case T_NUMBER:
		case T_INTEGER:
		case T_BOOLEAN:
		case T_UINTEGER:
		case T_NULL:
		case T_UNDEFINED:
			asAtomHandler::setNumber(ret,wrk,o->toNumber());
			break;
		case T_QNAME:
		case T_NAMESPACE:
			asAtomHandler::setNumber(ret,wrk,Number::NaN);
			break;
		default:
			o->incRef();
			ret = asAtomHandler::fromObject(o.getPtr());
			break;
	}
}

avmplusDomain::avmplusDomain(ASWorker* wrk, Class_base* c):
	ASObject(wrk,c)
{
}

void avmplusDomain::sinit(Class_base* c)
{
	CLASS_SETUP(c, ASObject,_constructor, CLASS_SEALED);
	c->setDeclaredMethodByQName("currentDomain","",Class<IFunction>::getFunction(c->getSystemState(),_getCurrentDomain),GETTER_METHOD,false);
	c->setDeclaredMethodByQName("MIN_DOMAIN_MEMORY_LENGTH","",Class<IFunction>::getFunction(c->getSystemState(),_getMinDomainMemoryLength),GETTER_METHOD,false);
	c->setDeclaredMethodByQName("load","",Class<IFunction>::getFunction(c->getSystemState(),load),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("loadBytes","",Class<IFunction>::getFunction(c->getSystemState(),loadBytes),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getClass","",Class<IFunction>::getFunction(c->getSystemState(),getClass),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("domainMemory","",Class<IFunction>::getFunction(c->getSystemState(),_getDomainMemory),GETTER_METHOD,true);
	c->setDeclaredMethodByQName("domainMemory","",Class<IFunction>::getFunction(c->getSystemState(),_setDomainMemory),SETTER_METHOD,true);
}
ASFUNCTIONBODY_ATOM(avmplusDomain,_constructor)
{
	_NR<avmplusDomain> parentDomain;
	ARG_UNPACK_ATOM(parentDomain);
	avmplusDomain* th = asAtomHandler::as<avmplusDomain>(obj);
	if (parentDomain.isNull())
		th->appdomain = ABCVm::getCurrentApplicationDomain(wrk->currentCallContext);
	else
		th->appdomain = _NR<ApplicationDomain>(Class<ApplicationDomain>::getInstanceS(wrk,parentDomain->appdomain));
}

ASFUNCTIONBODY_ATOM(avmplusDomain,_getCurrentDomain)
{
	avmplusDomain* res = Class<avmplusDomain>::getInstanceSNoArgs(wrk);
	res->appdomain = ABCVm::getCurrentApplicationDomain(wrk->currentCallContext);
	ret = asAtomHandler::fromObject(res);
}
ASFUNCTIONBODY_ATOM(avmplusDomain,_getMinDomainMemoryLength)
{
	asAtomHandler::setUInt(ret,wrk,MIN_DOMAIN_MEMORY_LIMIT);
}
ASFUNCTIONBODY_ATOM(avmplusDomain,load)
{
	avmplusDomain* th = asAtomHandler::as<avmplusDomain>(obj);
	tiny_string filename;
	uint32_t swfVersion;
	ARG_UNPACK_ATOM(filename)(swfVersion, 0);
	if (swfVersion != 0)
		LOG(LOG_NOT_IMPLEMENTED, "avmplus.Domain.load is unimplemented for swfVersion "<<swfVersion);
	if (!wrk->getSystemState()->getEngineData()->FileExists(wrk->getSystemState(),filename,false))
		throwError<ASError>(kFileOpenError,filename);
	_NR<ByteArray> bytes = _NR<ByteArray>(Class<ByteArray>::getInstanceS(wrk));
	wrk->getSystemState()->getEngineData()->FileReadByteArray(wrk->getSystemState(),filename,bytes.getPtr(),0,UINT32_MAX,false);

	// execute loaded abc file
	MemoryStreamCache mc(wrk->getSystemState());
	mc.append(bytes->getBuffer(bytes->getLength(),false),bytes->getLength());
	std::streambuf *sbuf = mc.createReader();
	std::istream s(sbuf);
	RootMovieClip* root=wrk->currentCallContext->mi->context->root;
	_NR<ApplicationDomain> origdomain = root->applicationDomain;
	root->applicationDomain = th->appdomain;
	ABCContext* context = new ABCContext(root, s, getVm(root->getSystemState()));
	context->exec(false);
	root->applicationDomain = origdomain;
	delete sbuf;
}
ASFUNCTIONBODY_ATOM(avmplusDomain,loadBytes)
{
	avmplusDomain* th = asAtomHandler::as<avmplusDomain>(obj);
	_NR<ByteArray> bytes;
	uint32_t swfversion;
	ARG_UNPACK_ATOM (bytes)(swfversion, 0);

	if (swfversion != 0)
		LOG(LOG_NOT_IMPLEMENTED,"Domain.loadBytes ignores parameter swfVersion");
	MemoryStreamCache mc(wrk->getSystemState());
	mc.append(bytes->getBuffer(bytes->getLength(),false),bytes->getLength());
	std::streambuf *sbuf = mc.createReader();
	std::istream s(sbuf);
	
	// execute loaded abc bytes
	RootMovieClip* root=wrk->currentCallContext->mi->context->root;
	_NR<ApplicationDomain> origdomain = root->applicationDomain;
	root->applicationDomain = th->appdomain;
	ABCContext* context = new ABCContext(root, s, getVm(root->getSystemState()));
	context->exec(false);
	root->applicationDomain = origdomain;
	delete sbuf;
}
ASFUNCTIONBODY_ATOM(avmplusDomain,getClass)
{
	getDefinitionByName(ret,wrk,obj,args,argslen);
}
ASFUNCTIONBODY_ATOM(avmplusDomain,_getDomainMemory)
{
	avmplusDomain* th = asAtomHandler::as<avmplusDomain>(obj);
	if (th->appdomain->domainMemory.isNull())
	{
		asAtomHandler::setNull(ret);
		return;
	}
	th->appdomain->domainMemory->incRef();
	ret = asAtomHandler::fromObject(th->appdomain->domainMemory.getPtr());
}
ASFUNCTIONBODY_ATOM(avmplusDomain,_setDomainMemory)
{
	_NR<ByteArray> b;
	ARG_UNPACK_ATOM(b);
	avmplusDomain* th = asAtomHandler::as<avmplusDomain>(obj);
	
	if (b.isNull())
	{
		th->appdomain->domainMemory = b;
		th->appdomain->checkDomainMemory();
		return;
	}
		
	if (b->getLength() < MIN_DOMAIN_MEMORY_LIMIT)
		throwError<RangeError>(kEndOfFileError);
	th->appdomain->domainMemory = b;
	th->appdomain->checkDomainMemory();
}

ASFUNCTIONBODY_ATOM(lightspark,casi32)
{
	if (wrk->rootClip
			&& wrk->rootClip->applicationDomain
			&& wrk->rootClip->applicationDomain->domainMemory)
	{
		asAtom a = asAtomHandler::fromObject(wrk->rootClip->applicationDomain->domainMemory.getPtr());
		ByteArray::atomicCompareAndSwapIntAt(ret,wrk,a,args,argslen);
	}
	else
		asAtomHandler::setInt(ret,wrk,0);
}

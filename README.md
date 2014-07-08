

#C++ class to json
##概述
从idl中生成C++ class，而C++ class支持jsoncpp的tojson与fromjson操作，从而实现自动化json序列化与反序列化C++结构体的目的。

##例子
###idl定义
idl保存到in.h中
<pre>
#include <stdint.h>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
//京东库存服务的IDL

namespace jd{
namespace core{
namespace stock{

class CLockInventoryForPaiPaiReq{
	//商品id
	std::string	productId;
};
class CLockInventoryForPaiPaiResp{
	//结果
	int32_t retCode;
};

}
}
}
</pre>

###自动生成
./fishjson < in.h > out.h

###生成结果
<pre>
#ifndef __JD_CORE_STOCK_H__
#define __JD_CORE_STOCK_H__

#include <sstream>
#include "app_platform/intf_serializable.h"
#include "c2cplatform/library/the3/jsoncpp64/reader.h"
#include "c2cplatform/library/the3/jsoncpp64/value.h"
#include "c2cplatform/library/the3/jsoncpp64/writer.h"

#include <stdint.h>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>

namespace jd{
namespace core{
namespace stock{

class CLockInventoryForPaiPaiReq{
private:
	std::string m_productId;
	bool m_productId_u;
public:
	CLockInventoryForPaiPaiReq():
		m_productId(),
		m_productId_u(false){
	}
	~CLockInventoryForPaiPaiReq(){
	}
public:
	const std::string& GetProductId()const{
		return m_productId;
	}
	bool IsProductIdSet()const{
		return m_productId_u;
	}
	void SetProductId( const std::string& productId){
		m_productId = productId;
		m_productId_u = true;
	}
public:
	void DumpData( std::ostream& os , int32_t indent = 0 )const{
		DumpDataTree( os ,indent );
	}
	void DumpDataTree( std::ostream& os ,int32_t indent = 0 )const{
		g_pprint(os,"productId",m_productId,indent);
	}
public:
	Json::Value ToJson()const{
		Json::Value root;
		std::stringstream ss;
		
		Json::Value root_1;
		if( m_productId_u == true ){
			root_1 = Json::Value(m_productId);
		}
		ss.str("");ss<<"productId";
		root[ss.str()] = root_1;

		return root;
	}
	void FromJson( const Json::Value& root ){
		if( root.isObject() == false ){
			return;
		}
		
		if( root["productId"].isString() == true ){
			m_productId = root["productId"].asString();
			m_productId_u = true;
		}

	}
};
class CLockInventoryForPaiPaiResp{
private:
	int32_t m_retCode;
	bool m_retCode_u;
public:
	CLockInventoryForPaiPaiResp():
		m_retCode(),
		m_retCode_u(false){
	}
	~CLockInventoryForPaiPaiResp(){
	}
public:
	const int32_t& GetRetCode()const{
		return m_retCode;
	}
	bool IsRetCodeSet()const{
		return m_retCode_u;
	}
	void SetRetCode( const int32_t& retCode){
		m_retCode = retCode;
		m_retCode_u = true;
	}
public:
	void DumpData( std::ostream& os , int32_t indent = 0 )const{
		DumpDataTree( os ,indent );
	}
	void DumpDataTree( std::ostream& os ,int32_t indent = 0 )const{
		g_pprint(os,"retCode",m_retCode,indent);
	}
public:
	Json::Value ToJson()const{
		Json::Value root;
		std::stringstream ss;
		
		Json::Value root_1;
		if( m_retCode_u == true ){
			root_1 = Json::Value(m_retCode);
		}
		ss.str("");
		ss<<"retCode";
		root[ss.str()] = root_1;

		return root;
	}
	void FromJson( const Json::Value& root ){
		if( root.isObject() == false ){
			return;
		}
		
		if( root["retCode"].isInt() == true ){
			m_retCode = root["retCode"].asInt();
			m_retCode_u = true;
		}

	}
};

}
}
}


#endif
</pre>

###接口
json序列化与反序列化操作
<pre>
int main(){
  CLockInventoryForPaiPaiReq req;
  Json::Value value1,value2;
  value1 = req.toJson();
  req.fromJson( value2 );
  return 0;
}
</pre>

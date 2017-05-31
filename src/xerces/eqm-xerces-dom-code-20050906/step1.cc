/*

This file is part of the sample code for the article,
"Processing XML with Xerces and the DOM" by Ethan McCallum (2005/09/08)

Published on ONLamp.com (http://www.onlamp.com/)
http://www.onlamp.com/pub/a/onlamp/2005/09/08/xerces_dom.html


step 1: demonstrate basic DOM with Xerces

Note that this code is far from optimal; please review the
refinements thereof (step2, step3).

*/


#include<xercesc/util/PlatformUtils.hpp>

#include<xercesc/dom/DOM.hpp>
#include<xercesc/dom/DOMDocument.hpp>
#include<xercesc/dom/DOMDocumentType.hpp>
#include<xercesc/dom/DOMElement.hpp>
#include<xercesc/dom/DOMImplementation.hpp>
#include<xercesc/dom/DOMImplementationLS.hpp>
#include<xercesc/dom/DOMNodeIterator.hpp>
#include<xercesc/dom/DOMNodeList.hpp>
#include<xercesc/dom/DOMText.hpp>

#include<xercesc/parsers/XercesDOMParser.hpp>
#include<xercesc/util/XMLUni.hpp>

#include<string>
#include<iostream>
#include<sstream>
#include<stdexcept>
#include<list>

enum {
	ERROR_ARGS = 1 ,
	ERROR_XERCES_INIT ,
	ERROR_PARSE ,
	ERROR_EMPTY_DOCUMENT
} ;



class TagNames {

	public:
	XMLCh* TAG_CONFIG ;
	XMLCh* TAG_LOGIN ;
	XMLCh* TAG_REPORTS ;
	XMLCh* TAG_REPORT ;

	XMLCh* ATTR_LASTUPDATE ;
	XMLCh* ATTR_USER ;
	XMLCh* ATTR_PASSWORD ;
	XMLCh* ATTR_NAME ;


	TagNames()
		:
		// we could have made these static members; but
		// we can't call XMLString::transcode() until
		// we've initialized Xerces...
		TAG_CONFIG( xercesc::XMLString::transcode( "config" ) ) ,
		TAG_LOGIN( xercesc::XMLString::transcode( "login" ) ) ,
		TAG_REPORTS( xercesc::XMLString::transcode( "reports" ) ) ,
		TAG_REPORT( xercesc::XMLString::transcode( "report" ) ) ,

		ATTR_LASTUPDATE( xercesc::XMLString::transcode( "lastupdate" ) ) ,
		ATTR_USER( xercesc::XMLString::transcode( "user" ) ) ,
		ATTR_PASSWORD( xercesc::XMLString::transcode( "password" ) ) ,
		ATTR_NAME( xercesc::XMLString::transcode( "name" ) )
	{
		return ;
	}


	~TagNames() throw(){

		try{
			xercesc::XMLString::release( &TAG_CONFIG ) ;
			xercesc::XMLString::release( &TAG_LOGIN ) ;
			xercesc::XMLString::release( &TAG_REPORTS ) ;
			xercesc::XMLString::release( &TAG_REPORT ) ;
	
			xercesc::XMLString::release( &ATTR_USER ) ;
			xercesc::XMLString::release( &ATTR_PASSWORD ) ;
		}catch( ... ){
			std::cerr << "Unknown exception encountered in TagNames dtor" << std::endl ;
		}
	
	} // dtor

} ; // class TagNames


// - - - - - - - - - - - - - - - - - - - -

class ConfigData {

	public:
	
	virtual void load() throw( std::runtime_error ) = 0 ;
	virtual void commit() throw( std::runtime_error ) = 0 ;

	virtual const std::string& getLastUpdate() const throw() = 0 ;

	virtual void setLastUpdate( const std::string& in ) = 0 ;
	virtual const std::string& getLoginUser() const throw() = 0 ;

	virtual void setLoginUser( const std::string& in ) = 0 ;
	virtual void setLoginPassword( const std::string& in ) = 0 ;

	virtual int getReportCount() const throw() = 0 ;
	virtual void addReport( const std::string& report ) = 0 ;

	virtual std::ostream& print( std::ostream& s ) const = 0 ;

	virtual ~ConfigData() throw() {}

	protected:
	ConfigData() {}

} ; // class ConfigData


std::ostream& operator<<( std::ostream& s , const ConfigData* obj ){

	return( obj->print( s ) ) ;

} // operator<< for ConfigData


// - - - - - - - - - - - - - - - - - - - -


class XMLConfigData : public ConfigData {

	private:

	typedef std::list< std::string > ReportList ;
	typedef ReportList::iterator ReportListIterator ;

	std::string xmlFile_ ;
	xercesc::XercesDOMParser parser_ ;
	std::string lastUpdate_ ;
	std::string loginUser_ ;
	std::string loginPassword_ ;
	ReportList reports_ ;
	const TagNames tags_ ;


	void handleElement( xercesc::DOMElement* element ){

		if( xercesc::XMLString::equals( tags_.TAG_LOGIN , element->getTagName() ) ){
	
			// get the user, password attrs

			// this will call functions on the underlying DOMAttr node for us...
			const XMLCh* loginUserXMLCh = element->getAttribute( tags_.ATTR_USER ) ;
			char* loginUserChar = xercesc::XMLString::transcode( loginUserXMLCh ) ;
			setLoginUser( loginUserChar ) ;
			xercesc::XMLString::release( &loginUserChar ) ;


			const XMLCh* userPasswordXMLCh = element->getAttribute( tags_.ATTR_PASSWORD ) ;
			char* userPasswordChar = xercesc::XMLString::transcode( userPasswordXMLCh ) ;
			setLoginPassword( userPasswordChar ) ;
			xercesc::XMLString::release( &userPasswordChar ) ;


		} else if( xercesc::XMLString::equals( tags_.TAG_REPORTS , element->getTagName() ) ){
	
			// fetch all child nodes that represent <report/> elements
			xercesc::DOMNodeList* reportNodes = element->getElementsByTagName( tags_.TAG_REPORT ) ;

			const XMLSize_t reportCount = reportNodes->getLength() ;
	
			for( XMLSize_t reportIndex = 0 ; reportIndex < reportCount ; ++reportIndex ){

				xercesc::DOMNode* reportNode = reportNodes->item( reportIndex ) ;

				// thanks to getElementsByTagName(), we already know these are
				// element attributes; so it's safe to blindly call dynamic_cast<>()
				xercesc::DOMElement* reportElement = dynamic_cast< xercesc::DOMElement* >( reportNode ) ;
				const XMLCh* reportNameXMLCh = reportElement->getAttribute( tags_.ATTR_NAME ) ;
				char* reportNameChar = xercesc::XMLString::transcode( reportNameXMLCh ) ; 

				addReport( reportNameChar ) ;

				xercesc::XMLString::release( &reportNameChar ) ;

			}


		} else {

			
			char* tagName = xercesc::XMLString::transcode( element->getTagName() ) ;

			std::ostringstream buf ;
			buf << "Unexpected tag: <" << tagName << "/>" << std::flush ;


			throw( std::runtime_error( buf.str() ) ) ;

		}

	} // handleElement()


	protected:
	// empty

	public:

	typedef ReportListIterator iterator ;

	XMLConfigData( const std::string& fromFile )
		:
		xmlFile_( fromFile ) ,
		parser_() ,
		lastUpdate_() ,
		loginUser_() ,
		loginPassword_() ,
		reports_() ,
		tags_()
	{
		return ;
	} // ctor


	~XMLConfigData() throw() {}

	void load()
		throw( std::runtime_error )
	{


		parser_.setValidationScheme( xercesc::XercesDOMParser::Val_Never ) ;
		parser_.setDoNamespaces( false ) ;
		parser_.setDoSchema( false ) ;
		parser_.setLoadExternalDTD( false ) ;
	
		try{
	
			parser_.parse( xmlFile_.c_str() ) ;

			// there's no need to free this pointer -- it's
			// owned by the parent parser object
			xercesc::DOMDocument* xmlDoc = parser_.getDocument() ;
	
			// We could also have called xml->getFirstChild() and worked
			// with DOMNode objects (DOMDocument is also a DOMNode); but
			// DOMNode only lets us get at the tree using other abstract
			// DOMNodes.  In turn, that would require us to walk the tree
			// and query each node for its name before we do anything with
			// the data.  getDocumentElement() gives us direct access to
			// the top-level element.
	
			// <config/> element
			xercesc::DOMElement* elementConfig = xmlDoc->getDocumentElement() ;
	
			if( NULL == elementConfig ){
				throw( std::runtime_error( "empty XML document" ) ) ;
			}
	
			// get the <config/> element's "lastupdate" attr
			// this will call functions on the underlying DOMAttr node for us...
			const XMLCh* lastUpdateXMLCh = elementConfig->getAttribute( tags_.ATTR_LASTUPDATE ) ;

			char* lastUpdateChar = xercesc::XMLString::transcode( lastUpdateXMLCh ) ;

			setLastUpdate( lastUpdateChar ) ;

			xercesc::XMLString::release( &lastUpdateChar ) ;

	
	
			// scroll through tags, looking for elements of interest
	
			// given that we know the document structure, we could have called
			// children->item() on (hard-coded) element index numbers; but that
			// would have been too rigid.
	
			xercesc::DOMNodeList* children = elementConfig->getChildNodes() ;
			const XMLSize_t nodeCount = children->getLength() ;
	
			for( XMLSize_t ix = 0 ; ix < nodeCount ; ++ix ){
	
				xercesc::DOMNode* currentNode = children->item( ix ) ;
	
				if( NULL == currentNode ){
					// null node...
					continue ;
				}
	
	
				if( xercesc::DOMNode::ELEMENT_NODE != currentNode->getNodeType() ){
					// not an element node -> not of interest here
					continue ;
				}
	
	
				// DOMElement has some special functions we're after: getting the
				// tag name, getting attributes, etc.

				// Given the above check, we should be able to safely cast "current"
				// to a DOMElement and without checking dynamic_cast<>()'s return value
	
				xercesc::DOMElement* currentElement = dynamic_cast< xercesc::DOMElement* >( currentNode ) ;
	
				handleElement( currentElement ) ;

			}
	
		}catch( xercesc::XMLException& e ){
	
			// believe it or not, XMLException is not
			// a parent class of DOMException
	
			char* message = xercesc::XMLString::transcode( e.getMessage() ) ;
	
			std::ostringstream buf ;
			buf << "Error parsing file: "
				<< message
				<< std::flush
			;
	
			xercesc::XMLString::release( &message ) ;
	
			throw( std::runtime_error( buf.str() ) ) ;

		}catch( const xercesc::DOMException& e ){
	
			char* message = xercesc::XMLString::transcode( e.getMessage() ) ;
	
			std::ostringstream buf ;
			buf << "Encountered DOM Exception: "
				<< message
				<< std::flush
			;
	
			xercesc::XMLString::release( &message ) ;
	
	
			throw( std::runtime_error( buf.str() ) ) ;
	
	    }
	
		return ;
	
	} // load()


	void commit()
		throw( std::runtime_error )
	{
		// next example
	} // commit()


	std::ostream& print( std::ostream& s ) const {
		s << "[ConfigData: "
			<< "lastUpdate=\"" << getLastUpdate() << "\"/"
			<< "user=\"" << getLoginUser() << "\"/"
			<< "pass=\"" << getLoginPassword() << "\"/"
			<< "reportCount=" << getReportCount()
			<< "]"
			<< std::flush
		;

		return( s ) ;
	}



	// fetch the individual reports

	iterator begin() {
		return( reports_.begin() ) ;
	} // begin()

	iterator end() {
		return( reports_.end() ) ;
	} // end()



	// basic accessors + mutators here till class end

	const std::string& getLastUpdate() const throw() {
		return( lastUpdate_ ) ;
	} 

	void setLastUpdate( const std::string& in ){
		lastUpdate_ = in ;
	} 



	const std::string& getLoginUser() const throw() {
		return( loginUser_ ) ;
	} 

	void setLoginUser( const std::string& in ){
		loginUser_ = in ;
	}



	const std::string& getLoginPassword() const throw() {
		return( loginPassword_ ) ;
	} 

	void setLoginPassword( const std::string& in ){
		loginPassword_ = in ;
	}



	int getReportCount() const throw() {
		return( reports_.size() ) ;
	} // getDocCount()

	void addReport( const std::string& report ){
		reports_.push_back( report ) ;
	} // addDoc()


} ; // class XMLConfigData



int main( const int argc , const char** argv ){

	if( 2 != argc ){

		std::cerr << "Usage: " << argv[0] << " [input file]" << std::endl ;
		return( ERROR_ARGS ) ;

	}

	const char* configFile = argv[1] ;

	try{

		xercesc::XMLPlatformUtils::Initialize();

	}catch( xercesc::XMLException& e ){

		char* message = xercesc::XMLString::transcode( e.getMessage() ) ;

		std::cerr << "XML toolkit initialization error: "
			<< message
			<< std::endl
		;

		xercesc::XMLString::release( &message ) ;

		return( ERROR_XERCES_INIT ) ;

	}


	ConfigData* config = new XMLConfigData( configFile ) ;

	try{

		config->load() ;

		std::cout << config << std::endl ;

	}catch( const std::runtime_error e ){
		std::cerr << "Error loading config: "
			<< e.what()
			<< std::endl
		;

	}


	try{
		xercesc::XMLPlatformUtils::Terminate();
	}catch( xercesc::XMLException& e ){

		char* message = xercesc::XMLString::transcode( e.getMessage() ) ;

		std::cerr << "XML toolkit teardown error: "
			<< message
			<< std::endl
		;

		xercesc::XMLString::release( &message ) ;
	}


} // main()


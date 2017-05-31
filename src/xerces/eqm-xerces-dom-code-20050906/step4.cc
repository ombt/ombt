/*

This file is part of the sample code for the article,
"Processing XML with Xerces and the DOM" by Ethan McCallum (2005/09/08)

Published on ONLamp.com (http://www.onlamp.com/)
http://www.onlamp.com/pub/a/onlamp/2005/09/08/xerces_dom.html


step 4: demonstrate using the DOM to write out well-formed XML.

*/


#include<helper-classes.h>

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

#include<xercesc/dom/DOMWriterFilter.hpp>
#include<xercesc/dom/DOMWriter.hpp>
#include<xercesc/framework/LocalFileFormatTarget.hpp>
#include<xercesc/framework/StdOutFormatTarget.hpp>


#include<xercesc/parsers/XercesDOMParser.hpp>
#include<xercesc/util/XMLUni.hpp>

#include<string>
#include<iostream>
#include<sstream>
#include<stdexcept>
#include<deque>

#include<ctime> // time()


enum {
	ERROR_ARGS = 1 ,
	ERROR_XERCES_INIT ,
	ERROR_PARSE ,
	ERROR_XERCES_TERMINATE
} ;

/*
takes care of finding elements in the tree.

For code/design purity, perhaps this class should manage the parent parser
or DOMDocument objects; but that would require some other methods, etc
so we leave that for another day.
*/

class ElementFinder {


	public:

	const DualString TAG_CONFIG_ ;
	const DualString TAG_LOGIN_ ;
	const DualString TAG_REPORTS_ ;
	const DualString TAG_REPORT_ ;

	const DualString ATTR_LASTUPDATE_ ;
	const DualString ATTR_USER_ ;
	const DualString ATTR_PASSWORD_ ;
	const DualString ATTR_NAME_ ;
	const DualString ATTR_TAB_ ;

	ElementFinder()
		:

		// we could have made these static members; but
		// DualString can't call XMLString::transcode() until
		// we've initialized Xerces...
		TAG_CONFIG_( "config" ) ,
		TAG_LOGIN_( "login" ) ,
		TAG_REPORTS_( "reports" ) ,
		TAG_REPORT_( "report" ) ,

		ATTR_LASTUPDATE_( "lastupdate" ) ,
		ATTR_USER_( "user" ) ,
		ATTR_PASSWORD_( "password" ) ,
		ATTR_NAME_( "name" ) ,
		ATTR_TAB_( "tab" ) ,
		xmlDoc_( NULL )

	{
		return ;
	}


	xercesc::DOMElement* getConfigElement(){

			// We could also have called xml->getFirstChild() and worked
			// with DOMNode objects (DOMDocument is also a DOMNode); but
			// DOMNode only lets us get at the tree using other abstract
			// DOMNodes.  In turn, that would require us to walk the tree
			// and query each node for its name before we do anything with
			// the data.
	
			// <config/> element
			xercesc::DOMElement* result = xmlDoc_->getDocumentElement() ;
	
			return( result ) ;	
	}


	xercesc::DOMElement* getLoginElement(){
		return( getElement( TAG_LOGIN_.asXMLString() ) ) ;
	}

	xercesc::DOMElement* getReportsElement(){

		return( getElement( TAG_REPORTS_.asXMLString() ) ) ;
	}


	xercesc::DOMElement* getElement( const XMLCh* name ){

		xercesc::DOMElement* result = NULL ;

		xercesc::DOMNodeList* list = xmlDoc_->getElementsByTagName( name ) ;


		xercesc::DOMNode* node = list->item( 0 ) ;

		if( xercesc::DOMNode::ELEMENT_NODE == node->getNodeType() ){
			result = dynamic_cast< xercesc::DOMElement* >( node ) ;
		}

		return( result ) ;

	}


	void setDocument( xercesc::DOMDocument* const doc ){
		xmlDoc_ = doc ;
	}


	protected:
	// empty


	private:
	xercesc::DOMDocument* xmlDoc_ ;

} ; // class ElementFinder



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


class XMLConfigData : public ConfigData {

	private:

	typedef std::deque< std::string > ReportList ;
	typedef ReportList::iterator ReportListIterator ;

	const static unsigned int MAX_REPORTS_ = 5 ;

	std::string xmlFile_ ;
	xercesc::XercesDOMParser parser_ ;
	std::string lastUpdate_ ;
	std::string loginUser_ ;
	std::string loginPassword_ ;
	ReportList reports_ ;
	ElementFinder finder_ ;

	void handleElement( const xercesc::DOMElement* element ){

		StringManager sm ;

		if( xercesc::XMLString::equals( finder_.TAG_LOGIN_.asXMLString() , element->getTagName() ) ){
	
			// get user, password attrs
			// this will call functions on the underlying DOMAttr node for us...
			const char* loginUser = sm.convert( element->getAttribute( finder_.ATTR_USER_.asXMLString() ) ) ;
			const char* userPassword = sm.convert( element->getAttribute( finder_.ATTR_PASSWORD_.asXMLString() ) ) ;

			setLoginUser( loginUser ) ;
			setLoginPassword( userPassword ) ;

		} else if( xercesc::XMLString::equals( finder_.TAG_CONFIG_.asXMLString() , element->getTagName() ) ){

			// get the "lastupdate" attr
			// this will call functions on the underlying DOMAttr node for us...
			DualString lastUpdate( element->getAttribute( finder_.ATTR_LASTUPDATE_.asXMLString() ) ) ;

			setLastUpdate( lastUpdate.asCString() ) ;



		} else if( xercesc::XMLString::equals( finder_.TAG_REPORTS_.asXMLString() , element->getTagName() ) ){
	
			// get a list of <report> elements
			xercesc::DOMNodeList* reportNodes = element->getElementsByTagName( finder_.TAG_REPORT_.asXMLString() ) ;

			const XMLSize_t reportCount = reportNodes->getLength() ;
	
			for( XMLSize_t reportIndex = 0 ; reportIndex < reportCount ; ++reportIndex ){

				xercesc::DOMNode* reportNode = reportNodes->item( reportIndex ) ;

				// thanks to getElementsByTagName(), we already know these are
				// element attributes
				xercesc::DOMElement* reportElement = dynamic_cast< xercesc::DOMElement* >( reportNode ) ;

				const char* reportName = sm.convert( reportElement->getAttribute( finder_.ATTR_NAME_.asXMLString() ) ) ;

				addReport( reportName ) ;

			}

		} else {

			
			std::ostringstream buf ;
			buf << "Unexpected tag: <" << DualString( element->getTagName() ) << "/>" << std::flush ;

			throw( std::runtime_error( buf.str() ) ) ;

		}

	}


	void updateObject(){

		// update config@lastupdate
		std::ostringstream configBuf ;
		configBuf << time( NULL ) << std::flush ;
		setLastUpdate( configBuf.str() ) ;

		return ;

	}


	void updateXMLTree(){

		StringManager sm ;

		xercesc::DOMElement* configElement = finder_.getConfigElement() ;
		configElement->setAttribute( finder_.ATTR_LASTUPDATE_.asXMLString() , sm.convert( getLastUpdate().c_str() ) ) ;


		xercesc::DOMDocument* xmlDoc = parser_.getDocument() ;
		xercesc::DOMElement* newReportsElement = xmlDoc->createElement( finder_.TAG_REPORTS_.asXMLString() )  ;

		const int length = reports_.size() ;


		std::ostringstream attrTabBuf ;

		for( int ix = 0 ; ix < length ; ++ix ){

			xercesc::DOMElement* element = xmlDoc->createElement( finder_.TAG_REPORT_.asXMLString() ) ;
 
			// likely, not the most efficient way...
			attrTabBuf.str( "" ) ;
			// tab numbering start at 1, not 0
			attrTabBuf << ix + 1 << std::flush ;
			
			element->setAttribute(
				finder_.ATTR_TAB_.asXMLString() ,
				sm.convert( attrTabBuf.str() )
			) ;
 
			
			element->setAttribute(
				finder_.ATTR_NAME_.asXMLString() ,
				sm.convert( reports_.at( ix ) )
			) ;
 

			newReportsElement->appendChild( element ) ;

		}

		xercesc::DOMElement* oldReportsElement = finder_.getReportsElement() ;
		configElement->replaceChild( newReportsElement , oldReportsElement ) ;
		// you don't have to free "oldReportsElement"; it's owned by the parent document
		
	}



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
		finder_()


	{
		return ;
	} // ctor

	~XMLConfigData() throw(){

		// notice, the calls to XMLString::release are gone --
		// that's handled in each DualString's destructor

	} // dtor


	void load()
		throw( std::runtime_error )
	{

		parser_.setValidationScheme( xercesc::XercesDOMParser::Val_Never ) ;
		parser_.setDoNamespaces( false ) ;
		parser_.setDoSchema( false ) ;
		parser_.setLoadExternalDTD( false ) ;
		// parser_.setIncludeIgnorableWhitespace( false ) ;
	
		StringManager sm ;

		try{
	
			parser_.parse( xmlFile_.c_str() ) ;

			// there's no need to free this pointer -- it's
			// owned by the parent parser object
			xercesc::DOMDocument* xmlDoc = parser_.getDocument() ;

			if( NULL == xmlDoc->getDocumentElement() ){
				throw( std::runtime_error( "empty XML document" ) ) ;
			}

			finder_.setDocument( xmlDoc ) ;

			handleElement( finder_.getConfigElement() ) ;	
			handleElement( finder_.getLoginElement() ) ;
			handleElement( finder_.getReportsElement() ) ;
	
		}catch( xercesc::XMLException& e ){
	
			// believe it or not, XMLException is not
			// a parent class of DOMException
	
			std::ostringstream buf ;
			buf << "Error parsing file: "
				<< DualString( e.getMessage() )
				<< std::flush
			;
	
			throw( std::runtime_error( buf.str() ) ) ;
	
	
		}catch( const xercesc::DOMException& e ){
	
			std::ostringstream buf ;
			buf << "Encountered DOM Exception: "
				<< DualString( e.getMessage() )
				<< std::flush
			;
	
			throw( std::runtime_error( buf.str() ) ) ;
	
	    }
	
		return ;
	
	} // load()


	void commit()
		throw( std::runtime_error )
	{

		try{

			updateObject() ;
			updateXMLTree() ;


			// other XMLFormatTarget impls: MemBufFormatTarget, StdOutFormatTarget

			/*
			for good or ill, this ctor creates the file on the spot;
			so if the program bails before anything is written to the
			target file, you can have a lot of zero-byte files laying
			around.

			More robust code would first write to a temp file, then move
			it into place after a successful write.
			*/
			xercesc::XMLFormatTarget* outfile = new xercesc::LocalFileFormatTarget( DualString( xmlFile_ ).asXMLString() ) ;

			// write it out
			// DOMWriter* writer = xmlDoc_->getImplementation()->createDOMWriter();
			xercesc::DOMDocument* xmlDoc = parser_.getDocument() ;
			xercesc::DOMImplementation* impl = xmlDoc->getImplementation() ;

			if( NULL == impl ){
				throw( std::runtime_error( "DOMImplementation is null!" ) ) ;
			}

			xercesc::DOMWriter* writer = impl->createDOMWriter();

			// add spacing and such for human-readable output
			if( writer->canSetFeature( xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true ) ){
				writer->setFeature( xercesc::XMLUni::fgDOMWRTFormatPrettyPrint , true );
			}

			xmlDoc->normalizeDocument() ;

			writer->writeNode( outfile , *xmlDoc ) ;

			delete( outfile ) ;
			delete( writer ) ;

		}catch( const xercesc::XMLException& e ){
	
			// believe it or not, XMLException is not
			// a parent class of DOMException
	
			std::ostringstream buf ;
			buf << "Error storing file: "
				<< DualString( e.getMessage() )
				<< std::flush
			;
	
			throw( std::runtime_error( buf.str() ) ) ;
	
	
	    }catch( ... ){
			throw( std::runtime_error( "unknown exception caught" ) ) ;
		}


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

		if( MAX_REPORTS_ < reports_.size() ){
			reports_.pop_front() ;
		}

		return ;

	} // addDoc()


	iterator begin() {
		return( reports_.begin() ) ;
	} // begin()

	iterator end() {
		return( reports_.end() ) ;
	} // end()

} ; // class XMLConfigData





int main( int argc , const char** argv ){

	if( argc < 2 ){

		std::cerr << "Usage: " << argv[0] << " [input file] name1 [name2 ... nameN]" << std::endl ;
		return( ERROR_ARGS ) ;

	}

	const std::string configFile( argv[1] ) ;

	try{

		xercesc::XMLPlatformUtils::Initialize();

	}catch( xercesc::XMLException& e ){

		std::cerr << "XML toolkit initialization error: "
			<< DualString( e.getMessage() )
			<< std::endl
		;

		return( ERROR_XERCES_INIT ) ;

	}


	ConfigData* config = new XMLConfigData( configFile ) ;

	try{

		config->load() ;


	}catch( const std::runtime_error e ){
		std::cerr << "Error loading config: "
			<< e.what()
			<< std::endl
		;

		return( ERROR_PARSE ) ;

	}


	// add some reports, i.e. the last ones loaded by the user
	for( int ix = 2 ; ix < argc ; ++ix ){
		config->addReport( argv[ix] ) ;
	}

	std::cout << "Before: " << config << '\n' << std::endl ;
	config->commit() ;
	std::cout << '\n' << "After: " << config << std::endl ;
	std::cout << '\n' << "Be sure to check the file contents; they should be different now." << std::endl ;

	delete( config ) ;

	try{
		xercesc::XMLPlatformUtils::Terminate();
	}catch( xercesc::XMLException& e ){

		std::cerr << "XML toolkit teardown error: "
			<< DualString( e.getMessage() )
			<< std::endl
		;
		return( ERROR_XERCES_TERMINATE ) ;

	}


} // main()


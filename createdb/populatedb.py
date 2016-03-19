import mwparserfromhell
import xml.sax
import MySQLdb
from collections import defaultdict

conn = None

namespaces = {
        "media":-2,
        "special":-1,
        "":0,
        "talk":1,
        "user":2,
        "user_talk":3,
        "project":4,
        "project_talk":5,
        "file":6,
        "file_talk":7,
        "mediawiki":8,
        "mediawiki_talk":9,
        "template":10,
        "template_talk":11,
        "help":12,
        "help_talk":13,
        "category":14,
        "category_talk":15,
        }

def splitFromNs( title ):
    pts = title.split( ":", 2)
    if len( pts ) < 2:
        return 0, title
    global namespaces
    return namespaces.get( pts[0].lower(), -3 ), pts[1]


def initDB():
    global conn
    conn = MySQLdb.connect(host= "siddhartha",
                  user="write_user",
                  passwd="write_user_passwd",
                  db="wiki")
    x = conn.cursor()
    try:
        x.execute( "CREATE TABLE IF NOT EXISTS `page` ( \
          `page_id` int(8) unsigned NOT NULL AUTO_INCREMENT, \
          `page_namespace` int(11) NOT NULL DEFAULT '0', \
          `page_title` varbinary(255) NOT NULL DEFAULT '', \
          `page_is_redirect` tinyint(1) unsigned NOT NULL DEFAULT '0', \
          PRIMARY KEY (`page_id`), \
          UNIQUE KEY `name_title` (`page_namespace`,`page_title`), \
          KEY `page_title` (`page_title`) \
        ) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=binary" )
        x.execute( "CREATE TABLE IF NOT EXISTS `pagelinks` ( \
          `pl_from` int(8) unsigned NOT NULL DEFAULT '0', \
          `pl_namespace` int(11) NOT NULL DEFAULT '0', \
          `pl_title` varbinary(255) NOT NULL DEFAULT '', \
          `pl_from_namespace` int(11) NOT NULL DEFAULT '0', \
          `number_in_page` int(8) unsigned NOT NULL DEFAULT '0', \
          UNIQUE KEY `pl_from` (`pl_from`,`pl_namespace`,`number_in_page`,`pl_title`), \
          KEY `pl_namespace` (`pl_namespace`,`pl_title`,`pl_from`), \
          KEY `pl_backlinks_namespace` (`pl_from_namespace`,`pl_namespace`,`pl_title`,`pl_from`) \
        ) ENGINE=InnoDB DEFAULT CHARSET=binary" )
        conn.commit()
    except:
        conn.rollback()

def insert():
    global conn
def closeDB():
    if conn is not None:
        conn.close()

def getWikiLinks( text ):
    wikicode = mwparserfromhell.parse(text)
    wls = wikicode.filter_wikilinks()
    return [ splitFromNs( l[2:-2].split("|",1)[0].split("#")[0] ) for l in wls ]



class ArticlesHandler( xml.sax.ContentHandler ):
    def __init__( self ):
        self.resetPage()
        self.pagecount = 0

    def resetPage( self ):
        self.page_id = None
        self.page_title = None
        self.namespace = None
        self.text = None
        self.wikilinks = None
        self.content = ""
        self.redirect = 0
        self.stack = []

    def insertPage( self ):
        global conn
        x = conn.cursor()
        try:
            x.execute("INSERT INTO page VALUES (%s,%s,%s,%s)",(int(self.page_id),int(self.namespace),self.page_title,self.redirect))
            conn.commit()
        except:
            conn.rollback()

    def insertPageLinks( self ):
        global conn
        x = conn.cursor()
        try:
            x.executemany("INSERT INTO pagelinks VALUES (%s,%s,%s,%s,%s)",[ ( int(self.page_id), l[0], l[1], int(self.namespace), ind ) for ind, l in enumerate( self.wikilinks ) ] )
            conn.commit()
        except:
            conn.rollback()

    def isPageComplete( self ):
        return self.page_id is not None and \
            self.page_title is not None and \
            self.namespace is not None and \
            self.text is not None

    def skipPage( self ):
        return self.namespace != "0"

    def finishPage( self ):
        self.pagecount += 1
        if self.skipPage():
            self.resetPage()
        if self.isPageComplete():
            self.wikilinks = getWikiLinks( self.text )
            self.insertPage()
            self.insertPageLinks()
            # print str(self.page_id) + " " + self.page_title
            # print self.wikilinks
            # to sql
            # page: page_id page_title
            # pagelinks:
            # for l_title in self.wikilinks:
            #    pageid indexof( l ) l_title

        self.resetPage()
        if self.pagecount % 100 == 0:
            print self.pagecount

    def startElement( self, name, attrs ):
        self.content = ""
        self.stack.append( name )

    def endElement( self, name ):
        self.stack.pop()
        if name == "page":
            self.finishPage()
        if name == "id" and self.stack[-1] == "page":
            self.page_id = self.content
        elif name == "title":
            self.page_title = self.content
        elif name == "ns":
            self.namespace = self.content
        elif name == "text":
            self.text = self.content
        elif name == "redirect":
            self.redirect = 1


    def characters(self, content):
        self.content += content

def parseArticlesXml( filename ):
    parser = xml.sax.make_parser()
    parser.setContentHandler( ArticlesHandler() )
    parser.parse( open( filename, "r" ) )

def main():
    initDB()
    text = "I has [[]] [[ ]]a [[temp:late]]! [[user:foo#dart|bar]] [[See]]ing [[[it?"
    print getWikiLinks( text )
    # parseArticlesXml( "huwiki-20160203-pages-articles.xml" )
    parseArticlesXml( "elwiktionary-20160203-pages-articles.xml" )
    closeDB()

if __name__ == "__main__":
    main()

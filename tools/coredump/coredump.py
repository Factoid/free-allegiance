from igc import *
import struct

class CSVEntityWriter:
    def write( self, f, entity, items = None, ignore = [] ):
        if items == None:
            items = entity.attribs.keys()
            
        for i in items:
            if entity.attribs.has_key( i ):
                f.write( '"' + str( entity.attribs[ i ] ) + '",' )
            else:
                f.write( ',' )

        if ignore == None:
            ignore = entity.attribs.keys()
            
        for name, value in entity.attribs.iteritems():
            if (not name in items) and (not name in ignore):
                f.write( '"' + str( value ) + '",' )
        f.write( "\n" )

class CSVWriter:
    def __init__(self, igcreader):
        self.reader = igcreader

    def write( self ):
        self.dir = self.reader.getCoreName() + '/'
        try:
            os.makedirs( self.dir )
        except OSError, err:
            if err.errno != errno.EEXIST:
                print "Fatal Error:", err
                return

        self.writeFactions()
        self.writeShips()
        self.writeParts()
        self.writeDmgTable()
        self.writeMissiles()
        self.writeLaunchers()
        self.writeMines()
        self.writeProbes()
        self.writeProjectiles()
        self.writeStations()
        self.writeDrones()
        self.writeChaff()
        self.writeTech()
        self.writeTreasure()
        self.writeChaffTable()

    def writeData( self, path, data, attribs = None, ignore = [] ):
        entWriter = CSVEntityWriter()
        
        print "Writing", path, len(data), "entities"
 
        if len(data) == 0:
            return

        attribs = None
        ignore = [] 
        if attribs == None:
            attribs = data[0].attribs.keys()
        if ignore == None:
            ignore = data[0].attribs.keys()

        attribs.sort()
        f = file( self.dir + path, "w" )
        for a in attribs:
            f.write( '"' + a + '",' )
        for key in data[0].attribs:
            if ( not key in attribs ) and ( not key in ignore ):
                f.write( '"' + key + '",' )
        f.write( "\n" )
        for d in data:
            entWriter.write( f, d, attribs, ignore )

    def writeShips( self ):
        attribs = [ "Name", "Sig", "Scan", "Ammo", "Rip Time" ]
#        ignore = [ "Description" ]
        ignore = []
        self.writeData( "ships.csv", self.reader.ships, attribs, ignore )
        
    def writeFactions( self ):
        attribs = [ "Name", "Ship Sensors", "Ship Signature" ]
        self.writeData( "factions.csv", self.reader.factions, attribs )

    def writeTech( self ):
        attribs = [ "Name" ]
        ignore = [ "Description" ]
        self.writeData( "tech.csv", self.reader.techs, attribs, ignore ) 

    def writeParts( self ):
        attribs = [ "Name" ]
        ignore = [ "Description", "Type" ]
        self.writeData( "parts.csv", self.reader.parts, attribs, ignore )
        self.writeData( "weapons.csv", self.reader.weapons, attribs, ignore )
        self.writeData( "shields.csv", self.reader.shields, attribs, ignore )
        self.writeData( "cloaks.csv", self.reader.cloaks, attribs, ignore )
        self.writeData( "boosters.csv", self.reader.boosters, attribs, ignore )

    def writeMissiles( self ):
        attribs = [ "Name" ]
        ignore = [ "Description", "Unused" ]
        self.writeData( "missiles.csv", self.reader.missiles, attribs, ignore )

    def writeLaunchers( self ):
        attribs = [ "Name" ]
        ignore = [ "Description", "Unused" ]
        self.writeData( "launchers.csv", self.reader.launchers, attribs, ignore )

    def writeMines( self ):
        attribs = [ "Name" ]
        ignore = [ "Description", "Unused" ]
        self.writeData( "mines.csv", self.reader.mines, attribs, ignore )

    def writeDrones( self ):
        attribs = [ "Name" ]
        ignore = [ "Description", "Unused" ]
        self.writeData( "drones.csv", self.reader.drones, attribs, ignore )
   
    def writeChaff( self ):
        attribs = [ "Name" ]
        ignore = [ "Description", "Unused" ]
        self.writeData( "chaff.csv", self.reader.chaff, attribs, ignore )
   
    def writeProjectiles( self ):
        attribs = [ "UID" ]
        self.writeData( "projectiles.csv", self.reader.projectiles, attribs )

    def writeProbes( self ):
        attribs = [ "Name" ]
        ignore = [ "Description", "Unused" ]
        self.writeData( "probes.csv", self.reader.probes, attribs, ignore )

    def writeStations( self ):
        attribs = [ "Name" ]
        ignore = [ "Description", "Unused" ]
        self.writeData( "stations.csv", self.reader.stations, attribs, ignore )
    
    def writeDmgTable( self ):
        if self.reader.const != None:
            print "Writing dmgtable.csv"
            f = file( self.dir + "dmgtable.csv", "w" )
            f.write( '"DC \ AC","asteroid (AC0)","light (AC1)","medium (AC2)","heavy (AC3)","extra heavy (AC4)","utility (AC5)","minor base hull (AC6)","major base hull (AC7)","lt & med shield (AC8)","minor base shld (AC9)","major base shld (AC10)","parts (AC11)","lt base hull (AC12)","lt base shld (AC13)","large shield (AC14)","AC15","AC16","AC17","AC18","AC19"\n' )
            for i in range(20):
                f.write( '"DC' + str(i) + '",' )
                for j in range(20):
                    f.write( '"' + str( self.reader.const.dmgTable[ i*20 + j ] ) + '",' )
                f.write( '\n' )

    def writeChaffTable( self ):
        print "Writing chaff table"
        f = file( self.dir + "chaff_table.csv", "w" )

        f.write( '"Missile \ Chaff",' )
        for c in self.reader.chaff:
            f.write( '"' + c.attribs["ld_name"] + '",' )

        for m in self.reader.missiles:
            f.write( "\n" )
            f.write( '"' + m.attribs["ld_name"] + '",' )
            for c in self.reader.chaff:
                ecm = c.attribs["strength"]
                resist = m.attribs["resist"]
                chance = 0
                if( ecm <= resist ):
                    chance = 0.5 * ecm / resist
                else:
                    chance = 1.0 - (0.5 * resist / ecm)
                f.write( "%0.2f, " % (chance) )

    def writeTreasure( self ):
        self.writeData( "treasure_set.csv", self.reader.treasureset )

for fileName in os.listdir( "." ):
    if( fileName[-4:] == ".igc" ):
        print "Dumping", fileName
        reader = IGCReader()
        reader.open( fileName )
        CSVWriter(reader).write()

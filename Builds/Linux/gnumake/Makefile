# Builds all the projects in the solution...
.PHONY: all_projects
all_projects: UnitTest Database Runtime Utility Network ServerTest ClientTest 

# Builds project 'UnitTest'...
.PHONY: UnitTest
UnitTest: Runtime Network Utility Database 
	make --directory="Projects/" --file=UnitTest.makefile

# Builds project 'Database'...
.PHONY: Database
Database: Runtime Utility 
	make --directory="Projects/" --file=Database.makefile

# Builds project 'Runtime'...
.PHONY: Runtime
Runtime: Utility 
	make --directory="Projects/" --file=Runtime.makefile

# Builds project 'Utility'...
.PHONY: Utility
Utility: 
	make --directory="Projects/" --file=Utility.makefile

# Builds project 'Network'...
.PHONY: Network
Network: Utility 
	make --directory="Projects/" --file=Network.makefile

# Builds project 'ServerTest'...
.PHONY: ServerTest
ServerTest: Runtime Network Utility Database 
	make --directory="Projects/" --file=ServerTest.makefile

# Builds project 'ClientTest'...
.PHONY: ClientTest
ClientTest: Runtime Network Utility Database 
	make --directory="Projects/" --file=ClientTest.makefile

# Cleans all projects...
.PHONY: clean
clean:
	make --directory="Projects/" --file=UnitTest.makefile clean
	make --directory="Projects/" --file=Database.makefile clean
	make --directory="Projects/" --file=Runtime.makefile clean
	make --directory="Projects/" --file=Utility.makefile clean
	make --directory="Projects/" --file=Network.makefile clean
	make --directory="Projects/" --file=ServerTest.makefile clean
	make --directory="Projects/" --file=ClientTest.makefile clean


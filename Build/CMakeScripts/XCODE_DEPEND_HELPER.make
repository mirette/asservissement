# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# For each target create a dummy rule so the target does not have to exist


# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.PlotterProject.Debug:
/Users/mirette/Documents/Polytech\ Paris\ Sud/2eme\ année/Tronc\ Commun/Projet\ Coop/Plotter_Project/Plotter/bin/Debug/PlotterProject_d:
	/bin/rm -f /Users/mirette/Documents/Polytech\ Paris\ Sud/2eme\ année/Tronc\ Commun/Projet\ Coop/Plotter_Project/Plotter/bin/Debug/PlotterProject_d


PostBuild.PlotterProject.Release:
/Users/mirette/Documents/Polytech\ Paris\ Sud/2eme\ année/Tronc\ Commun/Projet\ Coop/Plotter_Project/Plotter/bin/Release/PlotterProject:
	/bin/rm -f /Users/mirette/Documents/Polytech\ Paris\ Sud/2eme\ année/Tronc\ Commun/Projet\ Coop/Plotter_Project/Plotter/bin/Release/PlotterProject


PostBuild.PlotterProject.MinSizeRel:
/Users/mirette/Documents/Polytech\ Paris\ Sud/2eme\ année/Tronc\ Commun/Projet\ Coop/Plotter_Project/Plotter/bin/MinSizeRel/PlotterProject:
	/bin/rm -f /Users/mirette/Documents/Polytech\ Paris\ Sud/2eme\ année/Tronc\ Commun/Projet\ Coop/Plotter_Project/Plotter/bin/MinSizeRel/PlotterProject


PostBuild.PlotterProject.RelWithDebInfo:
/Users/mirette/Documents/Polytech\ Paris\ Sud/2eme\ année/Tronc\ Commun/Projet\ Coop/Plotter_Project/Plotter/bin/RelWithDebInfo/PlotterProject:
	/bin/rm -f /Users/mirette/Documents/Polytech\ Paris\ Sud/2eme\ année/Tronc\ Commun/Projet\ Coop/Plotter_Project/Plotter/bin/RelWithDebInfo/PlotterProject



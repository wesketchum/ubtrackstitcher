# basic source code CMakeLists.txt

art_make_library( LIBRARY_NAME  ub_TrackAnalysis
	SOURCE TrackAnalysis.cxx
	LIBRARIES
		Geometry
		RecoBase
                ${FHICLCPP}
                ${ROOT_BASIC_LIB_LIST}
                ${CETLIB}
                ${BOOST_SYSTEM} #why the hell do I need this?
)

install_headers()
install_source()

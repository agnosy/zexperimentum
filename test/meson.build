zframe_test = executable(
	'zframe-test',
	'zframe-test.c',
	include_directories : inc,
	dependencies : [ zyre_dep, czmq_dep ],
	link_with : zexperimentum
)

zproc_test = executable(
	'zproc-test',
	'zproc-test.c',
	include_directories : inc,
	dependencies : [ zyre_dep, czmq_dep ],
	link_with : zexperimentum
)

test('zexperimentum test i', zframe_test)
test('zexperimentum test ii', zproc_test)


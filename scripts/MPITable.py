mpi_categorization_header = ['ACCESS_MODE', 'ALLOC_MEM_NUM_BYTES', 'ARGUMENT_COUNT', 'ARGUMENT_LIST', 'ARRAY_LENGTH',
                             'ARRAY_LENGTH_NNI',
                             'ARRAY_LENGTH_PI', 'ASSERT', 'ATTRIBUTE_VAL', 'ATTRIBUTE_VAL_10', 'BUFFER', 'COLOR',
                             'COMMUNICATOR',
                             'COMM_COMPARISON', 'COMM_SIZE', 'COORDINATE', 'C_BUFFER', 'C_BUFFER2', 'Code', 'DATATYPE',
                             'DEGREE',
                             'DIMENSION', 'DISPLACEMENT', 'ERRHANDLER', 'ERROR_CLASS', 'ERROR_CODE', 'EXTRA_STATE',
                             'F90_COMM',
                             'F90_GROUP', 'FILE', 'FUNCTION', 'GROUP', 'INDEX', 'INFO', 'INFO_VALUE_LENGTH', 'KEY',
                             'KEYVAL', 'KEY_INDEX',
                             'LOCK_TYPE', 'LOGICAL', 'MESSAGE', 'NUM_BYTES', 'NUM_DIMS', 'OFFSET', 'OPERATION', 'ORDER',
                             'POLYDISPLACEMENT', 'POLYDISPLACEMENT_AINT_COUNT', 'POLYDISPLACEMENT_COUNT',
                             'POLYDISPOFFSET',
                             'POLYDTYPE_NUM_ELEM', 'POLYDTYPE_NUM_ELEM_NNI', 'POLYDTYPE_NUM_ELEM_PI',
                             'POLYDTYPE_STRIDE_BYTES',
                             'POLYFUNCTION', 'POLYNUM_BYTES', 'POLYNUM_BYTES_NNI', 'POLYRMA_DISPLACEMENT',
                             'POLYXFER_NUM_ELEM',
                             'POLYXFER_NUM_ELEM_NNI', 'PROFILE_LEVEL', 'RANK', 'RANK_NNI', 'REQUEST',
                             'RMA_DISPLACEMENT_NNI',
                             'SPLIT_TYPE', 'STATUS', 'STRING', 'STRING_ARRAY', 'STRING_LENGTH', 'TAG', 'THREAD_LEVEL',
                             'TOPOLOGY_TYPE',
                             'TYPECLASS', 'TYPECLASS_SIZE', 'UPDATE_MODE', 'VERSION', 'WEIGHT', 'WINDOW', 'WINDOW_SIZE',
                             'WIN_ATTACH_SIZE', 'analysis_successful', 'call', 'define_value', 'defined_token',
                             'newCOMMUNICATOR',
                             'newDATATYPE', 'newGROUP', 'newINFO', 'num_occurrences', 'openacc_pragma_used',
                             'openmp_pragma_used',
                             'params_by_define', 'src_location', 'src_location_line_number', 'version',
                             'ACCESS_MODE_CATEGORY',
                             'ALLOC_MEM_NUM_BYTES_CATEGORY', 'ARGUMENT_COUNT_CATEGORY', 'ARGUMENT_LIST_CATEGORY',
                             'ARRAY_LENGTH_CATEGORY',
                             'ARRAY_LENGTH_NNI_CATEGORY', 'ARRAY_LENGTH_PI_CATEGORY', 'ASSERT_CATEGORY',
                             'ATTRIBUTE_VAL_CATEGORY',
                             'ATTRIBUTE_VAL_10_CATEGORY', 'BUFFER_CATEGORY', 'COLOR_CATEGORY', 'COMMUNICATOR_CATEGORY',
                             'COMM_COMPARISON_CATEGORY', 'COMM_SIZE_CATEGORY', 'COORDINATE_CATEGORY',
                             'C_BUFFER_CATEGORY',
                             'C_BUFFER2_CATEGORY', 'DATATYPE_CATEGORY', 'DEGREE_CATEGORY', 'DIMENSION_CATEGORY',
                             'DISPLACEMENT_CATEGORY',
                             'ERRHANDLER_CATEGORY', 'ERROR_CLASS_CATEGORY', 'ERROR_CODE_CATEGORY',
                             'EXTRA_STATE_CATEGORY',
                             'F90_COMM_CATEGORY', 'F90_GROUP_CATEGORY', 'FILE_CATEGORY', 'FUNCTION_CATEGORY',
                             'GROUP_CATEGORY',
                             'INDEX_CATEGORY', 'INFO_CATEGORY', 'INFO_VALUE_LENGTH_CATEGORY', 'KEY_CATEGORY',
                             'KEYVAL_CATEGORY',
                             'KEY_INDEX_CATEGORY', 'LOCK_TYPE_CATEGORY', 'LOGICAL_CATEGORY', 'MESSAGE_CATEGORY',
                             'NUM_BYTES_CATEGORY',
                             'NUM_DIMS_CATEGORY', 'OFFSET_CATEGORY', 'OPERATION_CATEGORY', 'ORDER_CATEGORY',
                             'POLYDISPLACEMENT_CATEGORY',
                             'POLYDISPLACEMENT_AINT_COUNT_CATEGORY', 'POLYDISPLACEMENT_COUNT_CATEGORY',
                             'POLYDISPOFFSET_CATEGORY',
                             'POLYDTYPE_NUM_ELEM_CATEGORY', 'POLYDTYPE_NUM_ELEM_NNI_CATEGORY',
                             'POLYDTYPE_NUM_ELEM_PI_CATEGORY',
                             'POLYDTYPE_STRIDE_BYTES_CATEGORY', 'POLYFUNCTION_CATEGORY', 'POLYNUM_BYTES_CATEGORY',
                             'POLYNUM_BYTES_NNI_CATEGORY', 'POLYRMA_DISPLACEMENT_CATEGORY',
                             'POLYXFER_NUM_ELEM_CATEGORY',
                             'POLYXFER_NUM_ELEM_NNI_CATEGORY', 'PROFILE_LEVEL_CATEGORY', 'RANK_CATEGORY',
                             'RANK_NNI_CATEGORY',
                             'REQUEST_CATEGORY', 'RMA_DISPLACEMENT_NNI_CATEGORY', 'SPLIT_TYPE_CATEGORY',
                             'STATUS_CATEGORY',
                             'STRING_CATEGORY', 'STRING_ARRAY_CATEGORY', 'STRING_LENGTH_CATEGORY', 'TAG_CATEGORY',
                             'THREAD_LEVEL_CATEGORY', 'TOPOLOGY_TYPE_CATEGORY', 'TYPECLASS_CATEGORY',
                             'TYPECLASS_SIZE_CATEGORY',
                             'UPDATE_MODE_CATEGORY', 'VERSION_CATEGORY', 'WEIGHT_CATEGORY', 'WINDOW_CATEGORY',
                             'WINDOW_SIZE_CATEGORY',
                             'WIN_ATTACH_SIZE_CATEGORY']

def find_category_index(target_category):
    try:
        index = mpi_categorization_header.index(target_category)
        return index
    except ValueError:
        return -1  # Return -1 if the string is not found in the list


class MPIParamList:
    def __init__(self):
        self.mpi_param_list = dict()

    def callback_param_append(self, mpi_call, mpi_call_categories, mpi_call_types):
        for cat, typ in zip(mpi_call_categories, mpi_call_types):
            self.mpi_param_list[cat] = typ

class MPICallback:
    def __init__(self, mpi_params, callback_name):
        self.categories = mpi_categorization_header
        self.mpi_params = mpi_params
        self.name = callback_name

    def make(self):
        param_l = self.mpi_params.mpi_param_list
        print('void {}('.format(self.name))
        print('  {}* {}, '.format('const char', 'mpi_fun_name'))
        print('  {}* {}, '.format('const void', 'called_from'))
        for i, cat in enumerate(self.categories):
            if cat in param_l:
                type = param_l[cat].replace("[]", "*")
                print('  {}* {}, '.format(type, cat))
        print(')')

include_directories( ${STREAMING_RESAMPLE_IMAGE_SOURCE_DIR}/Code )

add_executable( itkStreamingResampleImageFilterTest
  itkStreamingResampleImageFilterTest.cxx
  )
target_link_libraries( itkStreamingResampleImageFilterTest
  ITKCommon
  ITKIO
  )

add_test( itkStreamingResampleImageFilterTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkStreamingResampleImageFilterTest
  --compare itkStreamingResampleImageFilterTestStreamed.mha
	    itkStreamingResampleImageFilterTestNotStreamed.mha
  itkStreamingResampleImageFilterTest 
    ${ITK_DATA_ROOT}/Input/HeadMRVolume.mhd
    itkStreamingResampleImageFilterTestStreamed.mha
    itkStreamingResampleImageFilterTestNotStreamed.mha
  )

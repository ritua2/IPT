      program ex1_serial

!***  Stencil update
!***  This is the serial version
!***    Employs a temporary variable (tmp) inside loop
!***    Employs a reduction operation on variable sum

!***  The algorithm that presets the input arrays and also the format for the sum
!***    are carefully selected to hide round-off discrepancies in the output

!***  All variables and unnamed constants are in double precision

!***  Compile with: ifort -xhost ex1_serial.f90

!***  Execution time is of the order of seconds
!***  Memory requirement is about 13.5 GB


        integer                              :: n = 30000
        real(8), dimension(:,:), allocatable :: x, y
        real(8)                              :: sum, tmp

        write (0,*) 'Allocate arrays'
        allocate (x(n,n), y(n,n))

!***  The input array is carefull filled to minimize round-off errors
        write (0,*) 'Preset input array (x)'
        do j=1, n
          do i=1, n
            x(i,j) = real(modulo((i+j), 3)) - 0.9999_8
          enddo
        enddo

        write (0,*) 'Start timer and calculation'
        call system_clock(ic1, ir)

        sum = 0.

        do j=2, n-1
          do i=2, n-1
            tmp    = 0.2_8 * (x(i,j) + x(i-1,j) + x(i+1,j) + x(i,j-1) + x(i,j+1))
            y(i,j) = tmp
            sum    = sum + tmp
          enddo
        enddo
        
        call system_clock(ic2, ir)
        write (0,*) 'Stop timer after calculation'
        write (0,*)
        write (0,'(a,f12.5,a)') 'Time to completion :: ', (real(ic2) - real(ic1)) / real(ir), ' seconds'

!***  The output format with 2 digits after the decimal point is carefully selected to hide round-off errors
        write (0,'(a,f12.2)')   'The total sum is   :: ', sum

      end program ex1_serial

#** WARNING WARNING WARNING
# * Don't take this file as a good example of processing object test yet.
# * It will be re-made using a better design when XML load is available.
# */

function res = TestFFT(Audio,Spectrum)
  MaxRelativeError = 10e-4;
  good = fft(Audio);
  RelativeError = max(abs((good(1:length(Spectrum))-(Spectrum))./good(1:length(Spectrum))));
  if RelativeError > MaxRelativeError
	printf(" Error too big: %f !",RelativeError); 
	res=1;
  else
	printf(" Err=%f ",RelativeError); 
	res=0;
  endif
endfunction

load FFTTest.data

exit(TestFFT(Audio,Spectrum))

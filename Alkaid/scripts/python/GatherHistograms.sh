discriminant=("BDT_1bin" "BDT_2bin" "BDT_4bin" "Custom01_1bin" "Custom01_2bin" "Custom01_4bin" "Custom02_1bin" "Custom02_2bin" "Custom02_4bin" "Custom03_1bin" "Custom03_2bin" "Custom03_4bin")
coupling=("Agg" "Avv" "Hvv")
path="/afs/cern.ch/user/c/clcheng/ATLAS/Alkaid/output_LL/"


for d in "${discriminant[@]}"; do
for c in "${coupling[@]}"; do
pdfjam $path/$d/$c*.pdf --nup 2x1 --landscape --outfile $path/$d/$c_combined.pdf
done
done

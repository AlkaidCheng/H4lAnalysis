
#discriminant=("BDT_1bin" "BDT_2bin" "BDT_4bin" "Custom01_1bin" "Custom01_2bin" "Custom01_4bin" "Custom02_1bin" "Custom02_2bin" "Custom02_4bin" "Custom03_1bin" "Custom03_2bin" "Custom03_4bin")

discriminant=("BDT_4bin" )

base_path="/afs/cern.ch/user/c/clcheng/ATLAS/Alkaid/"
input_path=${base_path}"All_Input/"
output_path=${base_path}"output/"
input_sample_path=${input_path}"input_sample/"
input_xml_path=${input_path}"input_xml/"
workspace_xml_path=${input_path}"workspace_xml/"
workspace_path=${input_path}"workspace/"

for d in "${discriminant[@]}"; do
#python KinematicPlots.py ./input/input_ggf.txt ./input/param_create_input_${d}.txt -o="~/ATLAS/Alkaid/All_Input/input_sample/${d}/"
#python Create_Inputs.py $input_sample_path --discriminant=$d
#python writeXML.py --err="" --sample=${input_xml_path}"samples_BSM_kagg.txt" --category=${input_xml_path}"category.txt" --norm=${input_xml_path}"norm_factor_EFT_kagg.txt" --syst=${input_xml_path}"systematics_fake.txt" --discriminant=$d -o=${workspace_xml_path}"kagg_Asimov/" --Asimov --dtd=${input_xml_path}"HistFactorySchema.dtd" --wspath=${workspace_path}"kagg_Asimov/"  --mode="ggf"
#python writeXML.py --err="" --sample=${input_xml_path}"samples_BSM_kagg.txt" --category=${input_xml_path}"category.txt" --norm=${input_xml_path}"norm_factor_EFT_kagg.txt" --syst=${input_xml_path}"systematics_fake.txt" --discriminant=$d -o=${workspace_xml_path}"kagg_obs/" --dtd=${input_xml_path}"HistFactorySchema.dtd" --wspath=${workspace_path}"kagg_obs/"  --mode="ggf"
python LikelihoodFit.py ${workspace_path}kagg_Asimov/$d/ --couplingtype="kagg" --Asimov --smfix -o=${output_path} --discriminant=$d
#python LikelihoodFit.py ${workspace_path}kagg_Asimov/$d/ --couplingtype="kagg" --Asimov -o=${output_path} --discriminant=$d
#python LikelihoodFit.py ${workspace_path}kagg_obs/$d/ --couplingtype="kagg" --smfix -o=${output_path} --discriminant=$d
#python LikelihoodFit.py ${workspace_path}kagg_obs/$d/ --couplingtype="kagg" -o=${output_path} --discriminant=$d
done



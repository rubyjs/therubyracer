require 'v8/retained_objects'

describe V8::RetainedObjects do
  let(:object) { Object.new }
  let(:objects) { V8::RetainedObjects.new }

  it "knows that something isn't retained" do
    expect(objects).not_to be_retaining object
  end

  describe "adding a reference to an object" do
    before do
      objects.add(object)
    end

    it "is now retained" do
      expect(objects).to be_retaining object
    end

    describe "removing the reference" do
      before do
        objects.remove(object)
      end
      it "is no longer retained" do
        expect(objects).to_not be_retaining object
      end
    end
    describe "adding another reference and then removing" do
      before do
        objects.add(object)
        objects.remove(object)
      end
      it "is still retained" do
        expect(objects).to be_retaining object
      end

      describe "removing one more time" do
        before do
          objects.remove(object)
        end
        it "is no longer retained" do
          expect(objects).to_not be_retaining object
        end
      end
    end
  end
end
